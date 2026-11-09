#!/usr/bin/env python3
"""treemap.py <report.json> <out.svg>: draws the unit map of an objdiff report as an SVG treemap.

Same look as the decomp.dev progress treemap. Units are laid out by code size and split into one tile per
function: green when it matches, gray when it is still assembly. Used by make report for the README;
hovering a tile shows the unit and function."""
import json, sys
from html import escape

W, H = 1024, 320
GREEN = ((0x00, 0xC6, 0x00), (0x00, 0x56, 0x00))
GRAY = ((0x35, 0x35, 0x35), (0x26, 0x26, 0x26))


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
            side = sum(a for a, _ in trial) / short
            worst = max(max(side * side / a, a / (side * side)) for a, _ in trial)
            if best is not None and worst > best:
                break
            row, best = trial, worst
        rest = rest[len(row):]
        side = sum(a for a, _ in row) / short
        pos = 0
        for a, d in row:
            length = a / side
            out.append((d, x, y + pos, side, length) if w >= h else (d, x + pos, y, length, side))
            pos += length
        if w >= h:
            x, w = x + side, w - side
        else:
            y, h = y + side, h - side
    return out


def main():
    report = json.load(open(sys.argv[1]))
    units = sorted(((int(u["measures"].get("total_code", 0)), u) for u in report["units"]), key=lambda i: -i[0])
    defs, rects = [], []
    for u, ux, uy, uw, uh in squarify(units, 0, 0, W, H):
        funcs = sorted(((int(f.get("size", 0)), f) for f in u.get("functions", [])), key=lambda i: -i[0])
        for f, x, y, w, h in squarify(funcs, ux, uy, uw, uh):
            i = len(rects)
            center, edge = ["#%02x%02x%02x" % c for c in (GREEN if f.get("fuzzy_match_percent", 0) >= 100 else GRAY)]
            r = max(w, h) / 2
            defs.append(f'<radialGradient id="u{i}" gradientUnits="userSpaceOnUse" cx="{x + w / 2:.2f}" cy="{y + h / 2:.2f}" '
                        f'fr="{r * 0.2:.2f}" r="{r:.2f}"><stop offset="0%" stop-color="{center}"/>'
                        f'<stop offset="100%" stop-color="{edge}"/></radialGradient>')
            rects.append(f'<rect class="unit" x="{x:.2f}" y="{y:.2f}" width="{w:.2f}" height="{h:.2f}" fill="url(#u{i})">'
                         f'<title>{escape(u["name"])}: {escape(f["name"])}</title></rect>')
    svg = (f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {W} {H}" width="{W}" height="{H}">'
           '<style>.unit { stroke: #000; stroke-width: 0.5; }</style>'
           + "".join(defs) + "".join(rects) + "</svg>\n")
    open(sys.argv[2], "w").write(svg)


if __name__ == "__main__":
    main()
