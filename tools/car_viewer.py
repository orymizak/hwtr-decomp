#!/usr/bin/env python3
"""Build a standalone 3D viewer for HWTR cars (stage 1: proxy model).

Reads every decompressed .CAR (part table, bounding box) and its .CWH (handling
data) from assets/files and writes one HTML page with the data embedded.  The
body is drawn as a box from the CAR bounding box and each wheel as a cylinder at
its real position and radius, so wheel spin, steering, braking and reverse can
be watched.  Real geometry needs the compressed command streams decoded first.

Game coordinates: inches, X right, Y forward (rear axle at Y=0), Z up.
"""
from __future__ import annotations

import argparse
import json
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from car_inspector import inspect  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
WHEEL_RADIUS_LIMIT = 60.0  # body parts have radii above 150; wheels are 10-24

# CWH field = car struct offset - 0x4C4 (see MECHANICS.md).
CWH_FIELDS = {
    "max_steer": 0x4CC,
    "mass": 0x4D0,
    "thrust_g": 0x4DC,
    "thrust_split": 0x4E0,
    "drag_cd": 0x4E4,
}


def car_entry(car_path: Path) -> dict:
    data = car_path.read_bytes()
    report = inspect(car_path)
    bbox = struct.unpack_from(">6f", data, 0x14)
    wheels = [p for p in report["parts"] if p["radius"] < WHEEL_RADIUS_LIMIT]
    # Wheels appear twice (two sets with their own materials); the first set is used.
    wheels = wheels[: len(wheels) // 2] or wheels
    entry = {
        "name": car_path.stem,
        "bbox_min": [round(v, 3) for v in bbox[:3]],
        "bbox_max": [round(v, 3) for v in bbox[3:]],
        "wheels": [
            {"part": w["index"], "pos": [round(v, 3) for v in w["translation"]], "radius": round(w["radius"], 3)}
            for w in wheels
        ],
    }
    cwh_path = car_path.with_suffix(".CWH")
    if cwh_path.exists():
        cwh = cwh_path.read_bytes()
        entry["handling"] = {
            name: round(struct.unpack_from(">f", cwh, field - 0x4C4)[0], 4) for name, field in CWH_FIELDS.items()
        }
    return entry


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--assets", type=Path, default=ROOT / "assets" / "files", help="folder with .CAR/.CWH files")
    parser.add_argument("--output", type=Path, default=ROOT / "build" / "car_viewer.html", help="HTML page to write")
    args = parser.parse_args()

    cars = []
    for car_path in sorted(args.assets.glob("*.CAR")):
        try:
            cars.append(car_entry(car_path))
        except (OSError, ValueError, struct.error) as error:
            print(f"warning: skipping {car_path.name}: {error}", file=sys.stderr)
    if not cars:
        raise SystemExit(f"error: no .CAR files found in {args.assets}")

    template = (Path(__file__).resolve().parent / "car_viewer.html").read_text()
    page = template.replace("/*CAR_DATA*/[]", json.dumps(cars, separators=(",", ":")))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(page)
    print(f"{len(cars)} cars written to {args.output}")


if __name__ == "__main__":
    main()
