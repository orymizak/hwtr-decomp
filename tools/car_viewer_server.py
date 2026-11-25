#!/usr/bin/env python3
"""Serve car_viewer.html to several players on the network (standard library only).

Each browser simulates its own car and posts its state ~20 times per second; the server merges them
and streams everyone's cars to every browser with Server-Sent Events.  Environment changes (explosives
zone size, barrel radius, hidden barrels, kept wrecks, gravity, floor tunnelling, zone rebuilds,
detonations and wrecked zone cars) are shared: the server keeps the current settings and the events
since the last rebuild, and replays them to anyone who joins late.

    python3 tools/car_viewer_server.py                      # serves build/car_viewer.html on :8765
    python3 tools/car_viewer_server.py --page build/car_viewer_demo.html --port 9000

Remote players outside the local network: expose the port with a tunnel (e.g. `devtunnel host -p 8765`).
There is no authentication: anyone who can reach the port can join.
"""
from __future__ import annotations

import argparse
import json
import queue
import threading
import time
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
STALE_S = 5.0                 # a player that stops posting disappears after this
EVENT_LOG_MAX = 2000

lock = threading.Lock()
players: dict[int, dict] = {}          # id -> {"state": {...}, "seen": t}
listeners: dict[int, queue.Queue] = {}  # id -> outgoing messages
env = {"settings": {}, "events": []}   # shared environment
edits: dict[int, dict] = {}            # id -> the player's garage edit (paint, textures, shape, stats)
next_id = 1


def broadcast(message: dict, skip: int | None = None) -> None:
    data = json.dumps(message, separators=(",", ":"))
    with lock:
        targets = [q for pid, q in listeners.items() if pid != skip]
    for q in targets:
        q.put(data)


def ticker() -> None:
    """Send everyone's car states 20 times per second; drop players that went silent."""
    while True:
        time.sleep(0.05)
        now = time.time()
        with lock:
            for pid in [p for p, v in players.items() if now - v["seen"] > STALE_S]:
                players.pop(pid, None)
            states = {pid: v["state"] for pid, v in players.items()}
        broadcast({"type": "states", "players": states})


class Handler(BaseHTTPRequestHandler):
    page: Path = ROOT / "build" / "car_viewer.html"

    def log_message(self, fmt, *args):   # keep the console quiet
        pass

    def send_body(self, code: int, body: bytes, ctype: str) -> None:
        self.send_response(code)
        self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(body)

    def do_GET(self):
        global next_id
        if self.path in ("/", "/index.html"):
            html = self.page.read_text(encoding="utf-8")
            html = html.replace("<head>", "<head><script>window.HWTR_ONLINE = true;</script>", 1)
            self.send_body(200, html.encode("utf-8"), "text/html; charset=utf-8")
            return
        if self.path == "/events":
            with lock:
                pid = next_id
                next_id += 1
                q: queue.Queue = queue.Queue()
                listeners[pid] = q
                hello = {"type": "hello", "id": pid, "env": env["settings"], "events": list(env["events"]), "edits": dict(edits)}
            self.send_response(200)
            self.send_header("Content-Type", "text/event-stream")
            self.send_header("Cache-Control", "no-store")
            self.send_header("Connection", "keep-alive")
            self.send_header("X-Accel-Buffering", "no")       # ask proxies (e.g. tunnels) not to buffer
            self.end_headers()
            try:
                self.wfile.write(b": " + b" " * 2048 + b"\n\n")   # padding: some proxies hold small first chunks
                self.wfile.write(f"data: {json.dumps(hello)}\n\n".encode())
                self.wfile.flush()
                while True:
                    try:
                        data = q.get(timeout=15)
                        self.wfile.write(f"data: {data}\n\n".encode())
                    except queue.Empty:
                        self.wfile.write(b": keep-alive\n\n")
                    self.wfile.flush()
            except (BrokenPipeError, ConnectionResetError, OSError):
                pass
            finally:
                with lock:
                    listeners.pop(pid, None)
                    players.pop(pid, None)
                    edits.pop(pid, None)
                broadcast({"type": "left", "id": pid})
            return
        self.send_body(404, b"not found", "text/plain")

    def do_POST(self):
        length = int(self.headers.get("Content-Length", "0"))
        if length > (8 << 20 if self.path == "/edit" else 65536):    # edits carry repainted textures
            self.send_body(413, b"too large", "text/plain")
            return
        try:
            msg = json.loads(self.rfile.read(length) or b"{}")
        except json.JSONDecodeError:
            self.send_body(400, b"bad json", "text/plain")
            return
        pid = int(msg.get("id", 0))
        if self.path == "/state":
            with lock:
                players[pid] = {"state": msg.get("state", {}), "seen": time.time()}
        elif self.path == "/edit":
            with lock:
                edits[pid] = msg.get("edit") or {}
            broadcast({"type": "edit", "id": pid, "edit": edits[pid]}, skip=pid)
        elif self.path == "/event":
            event = msg.get("event", {})
            with lock:
                if event.get("type") == "env":
                    env["settings"].update(event.get("settings", {}))
                    if "pgSize" in event.get("settings", {}):
                        env["events"].clear()       # a new zone layout starts a new history
                elif event.get("type") == "rebuild":
                    env["events"].clear()
                elif event.get("type") == "hit":
                    pass                            # one-off player contact: not replayed to late joiners
                else:
                    env["events"].append(event)
                    del env["events"][:-EVENT_LOG_MAX]
            broadcast({"type": "event", "from": pid, "event": event}, skip=pid)
        self.send_body(204, b"", "text/plain")


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--page", type=Path, default=ROOT / "build" / "car_viewer.html", help="viewer page to serve")
    p.add_argument("--host", default="0.0.0.0", help="address to listen on (0.0.0.0 = the whole local network)")
    p.add_argument("--port", type=int, default=8765)
    a = p.parse_args()
    if not a.page.exists():
        raise SystemExit(f"error: {a.page} not found (run tools/car_viewer.py first)")
    Handler.page = a.page
    threading.Thread(target=ticker, daemon=True).start()
    server = ThreadingHTTPServer((a.host, a.port), Handler)
    server.daemon_threads = True
    print(f"serving {a.page.name} on http://{a.host}:{a.port}/  (Ctrl+C to stop)")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
