#!/usr/bin/env python3
"""treemap.py <report.json> <out.svg>: draws the unit map of an objdiff report as an SVG treemap.

Each zone (game, audio, ...) is a box sized by its code bytes; inside it, each unit is a tile sized by
its code and colored by how much of it is matched. Used by make report for the README."""
import json, sys
from html import escape

W, H = 960, 540
HEADER = 22


def squarify(items, x, y, w, h):
    """items: [(size, data)] sorted descending. Returns [(data, x, y, w, h)] (squarified layout)."""
    out = []
    items = [i for i in items if i[0] > 0]
    total = sum(s for s, _ in items)
    if not items or w <= 0 or h <= 0:
        return out
    scale = w * h / total
    rest = [(s * scale, d) for s, d in items]
    while rest:
        short = min(w, h)
        row, best = [], None
        for item in rest:
            trial = row + [item]
            area = sum(a for a, _ in trial)
            side = area / short
            worst = max(max(side * side / a, a / (side * side)) for a, _ in trial)
            if best is not None and worst > best:
                break
            row, best = trial, worst
        rest = rest[len(row):]
        area = sum(a for a, _ in row)
        side = area / short
        pos = 0
        for a, d in row:
            length = a / side
            if w >= h:
                out.append((d, x, y + pos, side, length))
            else:
                out.append((d, x + pos, y, length, side))
            pos += length
        if w >= h:
            x, w = x + side, w - side
        else:
            y, h = y + side, h - side
    return out


def color(p):
    """0% dark red -> 100% green, through amber."""
    stops = [(0, (0x5a, 0x1e, 0x24)), (50, (0x9a, 0x6b, 0x1c)), (99.99, (0x4f, 0x8f, 0x3a)), (100, (0x2e, 0xa0, 0x43))]
    for (p0, c0), (p1, c1) in zip(stops, stops[1:]):
        if p <= p1:
            t = (p - p0) / (p1 - p0) if p1 > p0 else 1
            return "#%02x%02x%02x" % tuple(round(a + (b - a) * t) for a, b in zip(c0, c1))
    return "#2ea043"


def main():
    report = json.load(open(sys.argv[1]))
    zones = {}
    for u in report["units"]:
        size = int(u["measures"].get("total_code", 0))
        zone, _, name = u["name"].partition("/")
        zones.setdefault(zone, []).append((size, (name, u["measures"].get("matched_code_percent", 0.0), size)))
    order = sorted(((sum(s for s, _ in us), z) for z, us in zones.items()), reverse=True)
    pct = {c["id"]: c["measures"].get("matched_code_percent", 0.0) for c in report.get("categories", [])}
    svg = [f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}" '
           'font-family="-apple-system,Segoe UI,Helvetica,Arial,sans-serif">',
           f'<rect width="{W}" height="{H}" fill="#161b22"/>']
    for zone, zx, zy, zw, zh in squarify([(s, z) for s, z in order], 0, 0, W, H):
        svg.append(f'<rect x="{zx:.1f}" y="{zy:.1f}" width="{zw:.1f}" height="{zh:.1f}" fill="#0d1117" stroke="#161b22" stroke-width="2"/>')
        if zh > HEADER + 4 and zw > 40:
            svg.append(f'<text x="{zx + 6:.1f}" y="{zy + 15:.1f}" font-size="12" font-weight="600" fill="#e6edf3">'
                       f'{escape(zone)} {pct.get(zone, 0):.1f}%</text>')
            ux, uy, uw, uh = zx + 2, zy + HEADER, zw - 4, zh - HEADER - 2
        else:
            ux, uy, uw, uh = zx + 2, zy + 2, zw - 4, zh - 4
        units = sorted(zones[zone], key=lambda i: -i[0])
        for (name, p, size), x, y, w, h in squarify(units, ux, uy, uw, uh):
            svg.append(f'<rect x="{x:.1f}" y="{y:.1f}" width="{w:.1f}" height="{h:.1f}" fill="{color(p)}" '
                       f'stroke="#0d1117" stroke-width="1"><title>{escape(name)}: {p:.1f}% of {size} bytes</title></rect>')
            if w > 64 and h > 26:
                svg.append(f'<text x="{x + 4:.1f}" y="{y + 13:.1f}" font-size="10" fill="#e6edf3">{escape(name)}</text>'
                           f'<text x="{x + 4:.1f}" y="{y + 24:.1f}" font-size="9" fill="#c9d1d9">{p:.0f}%</text>')
    svg.append("</svg>")
    open(sys.argv[2], "w").write("\n".join(svg) + "\n")


if __name__ == "__main__":
    main()
