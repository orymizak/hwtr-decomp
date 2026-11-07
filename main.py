#!/usr/bin/env python3
"""Estado de la descompilación: compila, comprueba la ROM y muestra el avance. No es el juego.

python3 main.py                   muestra el estado en texto
python3 main.py --screenshot a.png  además lo dibuja en un PNG
python3 main.py --self-test a.json  guarda las comprobaciones en un JSON
Sale con 1 si la ROM no es idéntica o alguna función no coincide."""
import hashlib, json, os, re, struct, subprocess, sys, zlib
from pathlib import Path

ROM_SHA1 = "94913a07e49005ffd43dfdfa16ff5862bdb93748"
ROM = Path("build/hotwheelsturbo.z64")
WIDTH, HEIGHT, SCALE = 640, 360, 2
BACKGROUND, TEXT, DIM, BAR, TRACK = (18, 20, 28), (235, 235, 235), (150, 155, 170), (70, 190, 120), (50, 54, 68)
BAD = (225, 80, 70)

# Letra de 5x7, por columnas, con el bit bajo arriba.
FONT = {
    " ": "0000000000", "%": "2313086462", "-": "0808080808", ".": "0060600000", "/": "2010080402",
    ":": "0036360000", "0": "3E5149453E", "1": "00427F4000", "2": "4261514946", "3": "2141454B31",
    "4": "1814127F10", "5": "2745454539", "6": "3C4A494930", "7": "0171090503", "8": "3649494936",
    "9": "064949291E", "A": "7E1111117E", "B": "7F49494936", "C": "3E41414122", "D": "7F4141221C",
    "E": "7F49494941", "F": "7F09090901", "G": "3E4149497A", "H": "7F0808087F", "I": "00417F4100",
    "J": "2040413F01", "K": "7F08142241", "L": "7F40404040", "M": "7F020C027F", "N": "7F0408107F",
    "O": "3E4141413E", "P": "7F09090906", "Q": "3E4151215E", "R": "7F09192946", "S": "4649494931",
    "T": "01017F0101", "U": "3F4040403F", "V": "1F2040201F", "W": "3F4038403F", "X": "6314081463",
    "Y": "0708700807", "Z": "6151494543",
}


def run(*cmd):
    return subprocess.run(cmd, capture_output=True, text=True)


def collect():
    """Compila y comprueba de verdad; nada de esto se da por supuesto."""
    built = run("make", "-j8")
    sha1 = hashlib.sha1(ROM.read_bytes()).hexdigest() if ROM.exists() else ""
    funcs = run(sys.executable, "tools/match/verify_funcs.py")
    zones = []
    for line in run(sys.executable, "tools/progress.py").stdout.splitlines():
        m = re.match(r"(\w+)\s+(\d+)/(\d+)\s+(\d+)/(\d+)\s+([\d.]+)%", line)
        if m:
            zones.append(dict(name=m.group(1), done=int(m.group(2)), funcs=int(m.group(3)), percent=float(m.group(6))))
    return dict(built=built.returncode == 0 or bool(sha1), sha1=sha1, identical=sha1 == ROM_SHA1,
                funcs_ok=funcs.returncode == 0, funcs_text=funcs.stdout.strip(), zones=zones)


def draw(state):
    pixels = bytearray(bytes(BACKGROUND) * (WIDTH * HEIGHT))

    def rect(x, y, w, h, color):
        for row in range(y, min(y + h, HEIGHT)):
            start = (row * WIDTH + x) * 3
            pixels[start:start + w * 3] = bytes(color) * w

    def text(x, y, string, color=TEXT, scale=SCALE):
        for ch in string.upper():
            data = bytes.fromhex(FONT.get(ch, FONT[" "]))
            for col, bits in enumerate(data):
                for row in range(7):
                    if bits >> row & 1:
                        rect(x + col * scale, y + row * scale, scale, scale, color)
            x += 6 * scale

    text(20, 18, "HOT WHEELS TURBO RACING N64", scale=3)
    text(20, 50, "ESTADO DE LA DESCOMPILACION - NO ES EL JUEGO", DIM)
    y = 84
    text(20, y, "ROM: " + ("IDENTICA" if state["identical"] else "DISTINTA"), BAR if state["identical"] else BAD)
    text(240, y, "FUNCIONES: " + ("COINCIDEN" if state["funcs_ok"] else "NO COINCIDEN"), BAR if state["funcs_ok"] else BAD)
    y += 24
    text(20, y, "SHA-1 " + (state["sha1"] or "SIN ROM").upper()[:40], DIM)
    y += 34
    for zone in state["zones"]:
        text(20, y + 4, zone["name"])
        rect(130, y, 340, 22, TRACK)
        rect(130, y, int(340 * zone["percent"] / 100), 22, BAR)
        text(480, y + 4, f"{zone['percent']:.2f}%")
        y += 34
    return pixels


def write_png(path, pixels):
    def chunk(tag, data):
        return struct.pack(">I", len(data)) + tag + data + struct.pack(">I", zlib.crc32(tag + data))

    rows = b"".join(b"\0" + bytes(pixels[y * WIDTH * 3:(y + 1) * WIDTH * 3]) for y in range(HEIGHT))
    Path(path).parent.mkdir(parents=True, exist_ok=True)
    Path(path).write_bytes(b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", WIDTH, HEIGHT, 8, 2, 0, 0, 0))
                           + chunk(b"IDAT", zlib.compress(rows)) + chunk(b"IEND", b""))


def play():
    """Ejecuta la ROM compilada en el emulador y devuelve lo medido en la memoria del juego, o None sin emulador."""
    if not Path("tools/emu/input_script.dylib").exists():
        return None
    out = Path(".scratch/play.json")
    out.parent.mkdir(exist_ok=True)
    out.unlink(missing_ok=True)
    env = dict(os.environ, SDL_AUDIODRIVER="dummy", HW_MENU_FRAMES="1800")
    try:
        subprocess.run([sys.executable, "tools/emu_harness/run.py", str(ROM), str(out), "--shots", ".scratch/shots"],
                       capture_output=True, timeout=240, env=env)
    except subprocess.TimeoutExpired:
        return None
    return json.loads(out.read_text())["results"] if out.exists() else None


def play_steps(r):
    """move, act y hud solo pasan con una carrera real; nunca se dan por buenos sin medirlos."""
    if r is None:
        missing = "no se pudo ejecutar la ROM en el emulador (tools/emu_harness/setup.sh)"
        return [dict(step=s, passed=False, detail=missing) for s in ("move", "act", "hud")]
    boot = f"{r.get('frames', 0)} fotogramas, {r.get('image_swaps', 0)} cambios de imagen, {r.get('pad_reads', 0)} lecturas del mando"
    if "dist" not in r:
        reason = f"la ROM arranca ({boot}), pero no se llegó a una carrera"
        return [dict(step=s, passed=False, detail=reason) for s in ("move", "act", "hud")]
    return [
        dict(step="move", passed=r["dist"] > 1.0, detail=f"el coche recorrió {r['dist']:.0f} pulgadas"),
        dict(step="act", passed=r["gas"] > 0.5 and r["max_speed"] > 10.0,
             detail=f"acelerador {r['gas']:.2f}, velocidad máxima {r['max_speed']:.0f} pulgadas/s"),
        dict(step="hud", passed=bool(r.get("picture_not_black")), detail="captura de la carrera" + ("" if r.get("picture_not_black") else " en negro")),
    ]


def self_test(path, state, pixels):
    steps = play_steps(play()) + [
        dict(step="rom", passed=state["identical"], detail=f"SHA-1 {state['sha1'] or 'sin ROM'}"),
        dict(step="funciones", passed=state["funcs_ok"], detail=state["funcs_text"]),
        dict(step="estado", passed=bool(state["zones"]) and bytes(BACKGROUND) * (WIDTH * HEIGHT) != bytes(pixels),
             detail=f"{len(state['zones'])} zonas de avance dibujadas"),
    ]
    Path(path).parent.mkdir(parents=True, exist_ok=True)
    Path(path).write_text(json.dumps(dict(version=1, steps=steps), ensure_ascii=False, indent=2) + "\n")


def main():
    os.chdir(Path(__file__).resolve().parent)
    args = sys.argv[1:]
    state = collect()
    pixels = draw(state)
    if "--screenshot" in args:
        write_png(args[args.index("--screenshot") + 1], pixels)
    if "--self-test" in args:
        self_test(args[args.index("--self-test") + 1], state, pixels)
    print("ROM:", "idéntica" if state["identical"] else "distinta", state["sha1"])
    print(state["funcs_text"])
    for zone in state["zones"]:
        print(f"{zone['name']:10} {zone['done']:5}/{zone['funcs']:<5} {zone['percent']:6.2f} %")
    sys.exit(0 if state["identical"] and state["funcs_ok"] else 1)


main()
