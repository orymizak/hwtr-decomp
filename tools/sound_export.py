#!/usr/bin/env python3
"""Export the HWTR sound samples (libmus CCCSND.PTR + CCCSND.WBK) to WAV.

CCCSND.PTR is a libmus "N64 PtrTablesV2" bank: a header with the sample count
and offsets to the base notes (u8), detune values (f32) and the wave list; each
wave is an ALWaveTable (base and length in the .WBK, type, loop and ADPCM book).
CCCSND.WBK ("N64 WaveTables") holds the VADPCM frames: 9 bytes per 16 samples.

Each sample is written at its own natural rate:
    rate × 2^((basenote × 100 + detune) / 1200)
where basenote is a signed semitone offset (most samples 0; the long music
samples -12, i.e. stored at half rate) and detune is a signed byte in cents
(stored in the top byte of a 32-bit slot).  The game's output rate was not found
as a constant (osAiSetFrequency gets it from a runtime config); --rate defaults
to 16000 Hz, which is what sounds right by ear for the effects.
"""
from __future__ import annotations

import argparse
import json
import struct
import wave
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def u32(d: bytes, o: int) -> int: return struct.unpack_from(">I", d, o)[0]
def s16(d: bytes, o: int) -> int: return struct.unpack_from(">h", d, o)[0]


def read_bank(ptr: bytes) -> list[dict]:
    if not ptr.startswith(b"N64 PtrTables"):
        raise ValueError("not a libmus PtrTables bank")
    count, basenotes, detunes, waves = u32(ptr, 0x20), u32(ptr, 0x24), u32(ptr, 0x28), u32(ptr, 0x2C)
    out = []
    for i in range(count):
        w = u32(ptr, waves + i * 4)
        base, length, kind = u32(ptr, w), u32(ptr, w + 4), ptr[w + 8]
        loop_off, book_off = u32(ptr, w + 0xC), u32(ptr, w + 0x10)
        entry = {"index": i, "base": base, "length": length, "type": kind,
                 "basenote": struct.unpack_from(">b", ptr, basenotes + i)[0],
                 "detune_cents": struct.unpack_from(">b", ptr, detunes + i * 4)[0]}
        if kind == 0:  # AL_ADPCM_WAVE
            order, npred = u32(ptr, book_off), u32(ptr, book_off + 4)
            entry["book"] = [s16(ptr, book_off + 8 + k * 2) for k in range(order * npred * 8)]
            entry["order"], entry["npredictors"] = order, npred
            if loop_off:
                entry["loop"] = {"start": u32(ptr, loop_off), "end": u32(ptr, loop_off + 4), "count": u32(ptr, loop_off + 8)}
        out.append(entry)
    return out


def decode_vadpcm(data: bytes, book: list[int], order: int) -> list[int]:
    """Standard N64 VADPCM decoder (order 2): 9-byte frames of 16 samples."""
    if order != 2:
        raise ValueError(f"ADPCM order {order} not supported")
    out: list[int] = []
    last = [0] * 8
    for f in range(0, len(data) - 8, 9):
        header = data[f]
        scale, pred = 1 << (header >> 4), header & 0xF
        c1, c2 = book[pred * 16: pred * 16 + 8], book[pred * 16 + 8: pred * 16 + 16]
        for half in range(2):
            raw = data[f + 1 + half * 4: f + 5 + half * 4]
            tmp = []
            for k in range(8):
                n = (raw[k // 2] >> (4 if k % 2 == 0 else 0)) & 0xF
                tmp.append((n - 16 if n >= 8 else n) * scale)
            block = []
            for i in range(8):
                total = c1[i] * last[6] + c2[i] * last[7]
                for x in range(i):
                    total += tmp[i - 1 - x] * c2[x]
                v = ((tmp[i] << 11) + total) >> 11
                block.append(max(-32768, min(32767, v)))
            out += block
            last = block
    return out


def read_effects(bfx: bytes) -> list[dict]:
    """CCCSND.BFX (libmus effect bank, see Snd_LoadSfxBank): u32 count, ..., +0x14 offset of a u16
    wave-number → sample table, then count × {u32 offset, u32 priority}.  Each effect is a short
    libmus command stream; the ones used here are 81 (wave), 84 (envelope, 7 bytes), 9C (volume),
    A6 (pan), a note byte < 0x80 followed by its length (FF FF = held, 8x yy = 2 bytes, else 1),
    and 80 (end); 85/87 take no argument, A7 one, 86/A3 two.  Most effects play note 48 (the
    reference pitch); a few transpose by (note - 48) semitones."""
    count, table = u32(bfx, 0), u32(bfx, 0x14)
    waves = struct.unpack_from(f">{(len(bfx) - table) // 2}H", bfx, table)
    out = []
    for i in range(count):
        pos = u32(bfx, 0x18 + i * 8)
        fx = {"index": i, "priority": u32(bfx, 0x1C + i * 8)}
        while pos < len(bfx):
            c = bfx[pos]
            if c == 0x80:
                break
            if c == 0x81:
                fx["sample"] = waves[bfx[pos + 1]]; pos += 2
            elif c == 0x84:
                pos += 8
            elif c == 0x9C:
                fx["volume"] = bfx[pos + 1]; pos += 2
            elif c == 0xA6:
                fx["pan"] = bfx[pos + 1]; pos += 2
            elif c in (0x85, 0x87):          # argument-less state commands
                pos += 1
            elif c == 0xA7:                  # one argument
                pos += 2
            elif c in (0x86, 0xA3):          # two arguments (0xA3: pitch effect)
                fx.setdefault("extra", []).append(list(bfx[pos: pos + 3])); pos += 3
            elif c < 0x80:
                fx["note"] = c
                if bfx[pos + 1] == 0xFF and bfx[pos + 2] == 0xFF:
                    fx["length"] = None; pos += 3        # held until stopped
                elif bfx[pos + 1] & 0x80:
                    fx["length"] = ((bfx[pos + 1] & 0x7F) << 8) | bfx[pos + 2]; pos += 3
                else:
                    fx["length"] = bfx[pos + 1]; pos += 2
            else:
                raise ValueError(f"effect {i}: unknown command {c:#x}")
        out.append(fx)
    return out


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--assets", type=Path, default=ROOT / "assets" / "files")
    p.add_argument("--out", type=Path, default=ROOT / "assets" / "workbench" / "sound")
    p.add_argument("--rate", type=int, default=16000, help="audio output rate of the game (assumed)")
    a = p.parse_args()
    ptr = (a.assets / "CCCSND.PTR").read_bytes()
    wbk = (a.assets / "CCCSND.WBK").read_bytes()
    bank = read_bank(ptr)
    a.out.mkdir(parents=True, exist_ok=True)
    manifest = []
    for e in bank:
        if e["type"] != 0:
            print(f"sample {e['index']}: type {e['type']} skipped")
            continue
        pcm = decode_vadpcm(wbk[e["base"]: e["base"] + e["length"]], e["book"], e["order"])
        name = f"sample{e['index']:03d}.wav"
        natural = round(a.rate * 2 ** ((e["basenote"] * 100 + e["detune_cents"]) / 1200))
        with wave.open(str(a.out / name), "wb") as w:
            w.setnchannels(1); w.setsampwidth(2); w.setframerate(natural)
            w.writeframes(struct.pack(f"<{len(pcm)}h", *pcm))
        manifest.append({k: v for k, v in e.items() if k != "book"} | {"file": name, "samples": len(pcm),
                         "rate": natural, "seconds": round(len(pcm) / natural, 3)})
    effects = read_effects((a.assets / "CCCSND.BFX").read_bytes())
    (a.out / "samples.json").write_text(json.dumps({"rate": a.rate, "samples": manifest, "effects": effects}, indent=1))
    print(f"{len(manifest)} samples written to {a.out}")


if __name__ == "__main__":
    main()
