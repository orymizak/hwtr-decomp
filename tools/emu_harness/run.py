#!/usr/bin/env python3
"""run.py <rom> <salida.json> [--shots carpeta]: arranca la ROM en mupen64plus, avanza con un mando virtual y mide la carrera.

Todo se lee de la memoria del juego: coche 0 en D_80118630, velocidad en +0xBC, posición en +0x98 y número de
coches en D_8011B318. Con --shots guarda capturas. No escribe en la memoria del juego."""
import ctypes as C, json, os, struct, sys, time
from pathlib import Path

EMU = Path("tools/emu/mupen64plus/2.6.0").resolve()
INPUT = Path("tools/emu/input_script.dylib").resolve()
CFG = Path("tools/emu/config").resolve()
ROM_OPEN, EXECUTE, STOP, SET_FRAME_CB, READ_SCREEN = 1, 5, 6, 15, 18
RSP, GFX, AUDIO, INPUT_T = 1, 2, 3, 4

START, B, A, Z = 0x10, 0x40, 0x80, 0x20
CAR = 0x80118630
NUM_CARS = 0x8011B318
MENU_LIMIT = int(os.environ.get("HW_MENU_FRAMES", "6000"))


def main():
    rom, out = sys.argv[1], Path(sys.argv[2])
    shots = Path(sys.argv[sys.argv.index("--shots") + 1]) if "--shots" in sys.argv else None
    if shots:
        shots.mkdir(parents=True, exist_ok=True)
    CFG.mkdir(parents=True, exist_ok=True)
    core = C.CDLL(str(EMU / "lib/libmupen64plus.dylib"), mode=C.RTLD_GLOBAL)
    DEBUG = C.CFUNCTYPE(None, C.c_void_p, C.c_int, C.c_char_p)(
        lambda ctx, level, msg: print("core:", msg.decode("latin1"), flush=True) if level <= int(os.environ.get("HW_LOG", "2")) else None)
    err = core.CoreStartup(0x020001, str(CFG).encode(), str(EMU / "share/mupen64plus").encode(), None, DEBUG, None, None)
    if err:
        raise SystemExit(f"CoreStartup {err}")
    section = C.c_void_p()
    core.ConfigOpenSection(b"Core", C.byref(section))
    emu_mode = C.c_int(int(os.environ.get("HW_CPU", "0")))  # 0: intérprete puro, 1: intérprete con caché, 2: recompilador
    core.ConfigSetParameter(section, b"R4300Emulator", 2, C.byref(emu_mode))
    core.ConfigOpenSection(b"Video-General", C.byref(section))
    for key, value in ((b"ScreenWidth", 640), (b"ScreenHeight", 480), (b"Fullscreen", 0)):
        val = C.c_int(value)
        core.ConfigSetParameter(section, key, 2 if key != b"Fullscreen" else 3, C.byref(val))
    data = Path(rom).read_bytes()
    err = core.CoreDoCommand(ROM_OPEN, len(data), data)
    if err:
        raise SystemExit(f"ROM_OPEN {err}")
    plugins = []
    keep = []
    for kind, path in ((GFX, EMU / ("lib/mupen64plus/mupen64plus-video-%s.dylib" % os.environ.get("HW_VIDEO", "rice"))),
                       (AUDIO, EMU / "lib/mupen64plus/mupen64plus-audio-sdl.dylib"),
                       (INPUT_T, INPUT),
                       (RSP, EMU / "lib/mupen64plus/mupen64plus-rsp-hle.dylib")):
        lib = C.CDLL(str(path))
        keep.append(lib)
        start = lib.PluginStartup
        start.argtypes = [C.c_void_p, C.c_void_p, C.c_void_p]
        err = start(core._handle, None, DEBUG if kind != INPUT_T else None)
        if err:
            raise SystemExit(f"PluginStartup {path.name}: {err}")
        plugins.append((kind, lib))
    for kind, lib in plugins:
        err = core.CoreAttachPlugin(kind, C.c_void_p(lib._handle))
        if err:
            raise SystemExit(f"CoreAttachPlugin {kind}: {err}")
    input_lib = next(lib for kind, lib in plugins if kind == INPUT_T)
    input_lib.input_script_set.argtypes = [C.c_uint]
    input_lib.input_script_reads.restype = C.c_uint
    set_keys = input_lib.input_script_set
    core.DebugMemGetPointer.restype = C.c_void_p
    core.DebugMemGetPointer.argtypes = [C.c_int]
    ram = {}

    def r32(addr):
        # El puntero solo existe con el juego en marcha; la RAM va por palabras en el orden del anfitrión.
        if "words" not in ram:
            pointer = core.DebugMemGetPointer(1)
            if not pointer:
                state["results"]["fallo"] = "sin acceso a la RAM del emulador"
                core.CoreDoCommand(STOP, 0, None)
                return 0
            ram["words"] = C.cast(pointer, C.POINTER(C.c_uint32))
        return ram["words"][(addr & 0x7FFFFF) // 4]

    def rf(addr):
        return struct.unpack(">f", struct.pack(">I", r32(addr)))[0]

    def r8(addr):
        return r32(addr & ~3) >> (24 - 8 * (addr & 3)) & 0xFF

    def keys_for(a=False, b=False, start=False, x=0, y=0):
        return (A if a else 0) | (B if b else 0) | (START if start else 0) | ((x & 0xFF) << 16) | ((y & 0xFF) << 24)

    state = dict(frame=0, phase="menu", race_frame=None, log=[], results={})

    def screenshot(name):
        if not shots:
            return
        buf = (C.c_ubyte * (640 * 480 * 3))()
        if core.CoreDoCommand(READ_SCREEN, 0, buf) == 0:
            write_png(shots / name, 640, 480, bytes(buf))
            state["results"]["picture_not_black"] = any(bytes(buf))

    def dump_threads():
        import bisect, subprocess
        syms = []
        for line in subprocess.check_output(["mips-linux-gnu-nm", "-n", "build/hotwheelsturbo.elf"], text=True).splitlines():
            p = line.split()
            if len(p) == 3 and "NON_MATCHING" not in p[2] and not p[2].startswith("jtbl_L"):
                syms.append((int(p[0], 16), p[2]))
        addrs = [a for a, _ in syms]
        name = lambda v: syms[bisect.bisect_right(addrs, v) - 1][1] if v >= addrs[0] else hex(v)
        t = r32(0x800FB60C)
        n = 0
        while t and n < 12 and t >= 0x80000000:
            pc, ra, state16 = r32(t + 0x11C), r32(t + 0x104), r32(t + 0x10) >> 16
            sp, queue = r32(t + 0xF4), r32(t + 8)
            trail = []
            if sp >= 0x80000000:
                for a in range(sp, sp + 0x200, 4):
                    w = r32(a)
                    if 0x80060450 <= w < 0x800E7AA0 and r32(w - 8) >> 26 == 3 and name(w) not in trail:
                        trail.append(name(w))
            print(f"  hilo id={r32(t + 0x14)} pri={r32(t + 4)} estado={state16} cola={name(queue) if queue else 0}: {' < '.join(trail[:7])}", flush=True)
            t = r32(t + 0xC)
            n += 1

    def on_frame(index):
        f = state["frame"] = state["frame"] + 1
        origin_now = C.cast(core.DebugMemGetPointer(4), C.POINTER(C.c_uint32))[1]
        if origin_now != state.get("origin"):
            state["origin"] = origin_now
            state["swaps"] = state.get("swaps", 0) + 1
        cars = r32(NUM_CARS)
        if state["phase"] == "menu":
            if os.environ.get("HW_WATCH") and f % int(os.environ.get("HW_EVERY", "150")) == 0:
                print(f"f={f} " + " ".join(f"{a}={r32(int(a, 16)):08x}" for a in os.environ["HW_WATCH"].split(",")), flush=True)
            if os.environ.get("HW_TEXT") and f == int(os.environ["HW_TEXT"]):
                for sym in (0x80141234, 0x8014124C, 0x80141238, 0x80141250, 0x80141254, 0x80141248):
                    a, s = sym, ""
                    while len(s) < 60 and r8(a):
                        s += chr(r8(a))
                        a += 1
                    p = r32(sym)
                    s2 = ""
                    if 0x80000000 <= p < 0x80800000:
                        while len(s2) < 60 and r8(p):
                            s2 += chr(r8(p))
                            p += 1
                    print(f"texto {sym:08x}: {s!r} -> {s2!r}", flush=True)
            if os.environ.get("HW_STATE") and f % 150 == 0:
                print(f"f={f} menu61D2={r8(0x800F61D2)} 61D3={r8(0x800F61D3)} 3EA0={r8(0x80123EA0)} 4C94={r8(0x80124C94)} 4146={r8(0x80124146)} 5F28={r32(0x800F5F28):08x} 2102={r8(0x800F2102)}", flush=True)
            if f % 600 == 0:
                core.DebugMemGetPointer.argtypes = [C.c_int]
                vi = C.cast(core.DebugMemGetPointer(4), C.POINTER(C.c_uint32))
                origin = vi[1] & 0x7FFFFF
                fb = sum(1 for i in range(0, 76800, 37) if r32(0x80000000 + origin + i * 4) not in (0, 0x00010001))
                bss = hash(tuple(r32(a) for a in range(0x80101E70, 0x8014ECD0, 0x400)))
                print(f"lecturas_de_mando={input_lib.input_script_reads()} f={f} cambios_de_imagen={state.get('swaps', 0)} coches={cars} codigo={r32(0x80060400):08x} vi_origin={origin:06x} vi_status={vi[0]:08x} fb_no_vacio={fb}/2076 bss={bss & 0xFFFF:04x} pos=({rf(CAR + 0x98):.1f},{rf(CAR + 0x9C):.1f}) v={rf(CAR + 0xBC):.1f}", flush=True)
                screenshot(f"menu_{f:05d}.png")
                if os.environ.get("HW_THREADS"):
                    dump_threads()
            if not os.environ.get("HW_SWEEP") and not os.environ.get("HW_HOLD"):
                screen = r32(0x800F9F78)
                if screen != state.get("screen"):
                    print(f"f={f} pantalla={screen:08x} coches={cars}", flush=True)
                    state["screen"] = screen
                    state["log"].append(dict(frame=f, screen=f"{screen:08x}"))
                # Start en la portada; después, A para aceptar. La cruceta cambia la opción de cada menú.
                phase = f % 80
                if screen == 0x800F668C:
                    set_keys(START if phase < 20 else 0)
                else:
                    set_keys(A if phase < 20 else 0)
            elif os.environ.get("HW_SWEEP"):
                bit = (f // 60) % 16
                set_keys((1 << bit) if f % 60 < 20 else 0)
                cur = (r32(0x800F9F78), r32(0x800F64B0), r32(0x800F64B4), r32(0x800F64B8), r32(0x800F61D4))
                if cur != state.get("sweep"):
                    print(f"f={f} bit={bit} pantalla={cur[0]:08x} teclas={cur[1]:08x} {cur[2]:08x} {cur[3]:08x} 61D4={cur[4]:08x}", flush=True)
                    state["sweep"] = cur
            elif os.environ.get("HW_HOLD"):
                set_keys(int(os.environ["HW_HOLD"], 16) if f % 40 < 20 else 0)
            if 0 < cars <= 8 and rf(CAR + 0x98) != 0.0:
                state["phase"] = "wait_go"
                state["race_frame"] = f
                state["log"].append(dict(frame=f, what="carrera iniciada", cars=cars))
                print("carrera en", f, flush=True)
                set_keys(0)
            elif f > MENU_LIMIT:
                state["results"]["fallo"] = "no se llegó a la carrera"
                state["phase"] = "done"
                core.CoreDoCommand(STOP, 0, None)
            return
        rel = f - state["race_frame"]
        speed, pos = rf(CAR + 0xBC), (rf(CAR + 0x98), rf(CAR + 0x9C), rf(CAR + 0xA0))
        if rel == 1:
            state["start_pos"] = pos
        if rel % 120 == 0:
            print(f"rel={rel} v={speed:.1f} pos=({pos[0]:.0f},{pos[1]:.0f},{pos[2]:.0f}) gas={rf(CAR + 0x14):.2f} steer={rf(CAR + 0x10):.2f}", flush=True)
        # Espera la cuenta atrás con el acelerador pisado y después gira.
        if rel < 1500:
            set_keys(keys_for(a=True))
        if rel == 300:
            screenshot("race_300.png")
        state["max_speed"] = max(state.get("max_speed", 0.0), speed)
        state["gas_seen"] = max(state.get("gas_seen", 0.0), rf(CAR + 0x14))
        state["last_pos"] = pos
        if rel == 1500:
            screenshot("race_1500.png")
            dist = sum((a - b) ** 2 for a, b in zip(pos, state["start_pos"])) ** 0.5
            state["results"].update(dist=dist, max_speed=state["max_speed"], gas=state["gas_seen"], cars=cars)
            state["phase"] = "done"
            core.CoreDoCommand(STOP, 0, None)

    FRAME_CB = C.CFUNCTYPE(None, C.c_uint)(on_frame)
    core.CoreDoCommand(SET_FRAME_CB, 0, FRAME_CB)
    t0 = time.time()
    core.CoreDoCommand(EXECUTE, 0, None)
    state["results"]["seconds"] = round(time.time() - t0, 1)
    state["results"]["image_swaps"] = state.get("swaps", 0)
    state["results"]["pad_reads"] = input_lib.input_script_reads()
    state["results"]["frames"] = state["frame"]
    out.write_text(json.dumps(dict(results=state["results"], log=state["log"]), indent=2) + "\n")
    print(state["results"], flush=True)


def write_png(path, w, h, rgb):
    import zlib
    rows = b"".join(b"\0" + rgb[(h - 1 - y) * w * 3:(h - y) * w * 3] for y in range(h))
    def chunk(tag, d):
        return struct.pack(">I", len(d)) + tag + d + struct.pack(">I", zlib.crc32(tag + d))
    path.write_bytes(b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0)) + chunk(b"IDAT", zlib.compress(rows)) + chunk(b"IEND", b""))


main()
