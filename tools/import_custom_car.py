#!/usr/bin/env python3
"""Convert external 3D models (e.g. Octane) into an authentic Hot Wheels Turbo Racing car.

Uses real N64 Hot Wheels 5-spoke chrome wheels & tire textures, authentic die-cast
metallic lacquer reflections (ENVMAP1 sphere mapping pass), and Hot Wheels racing livery.
"""
from __future__ import annotations

import copy
import json
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from car_viewer import car_entry, CWH_FIELDS

# Authentic Hot Wheels die-cast paint palette (RGBA)
HW_COLORS = {
    "Base": [0, 85, 230, 255],          # Hot Wheels Electric Metallic Blue (primary paint)
    "Accent": [245, 245, 250, 255],     # Hot Wheels Classic Racing Stripe White
    "Grey metal": [215, 220, 230, 255], # Polished Chrome (Engine, Supercharger, Struts)
    "Rims": [220, 225, 235, 255],       # Polished Chrome details
    "Tires": [25, 27, 30, 255],         # Satin wheel-arch liner black
    "Grey Light 2": [70, 75, 82, 255],  # Mechanical linkage charcoal
    "Window": [15, 20, 32, 255],        # Glossy tinted canopy glass
    "Material": [40, 44, 50, 255],      # Undercarriage & bumper satin dark grey
    "Red": [240, 22, 22, 255],          # Hot Wheels Rocket Thruster / Taillight Ruby Red
    "Grey Light": [170, 175, 185, 255], # Brushed Aluminium grille
}

# Die-cast reflection intensity (alpha for ENVMAP1 pass, combine 4)
ENVMAP_ALPHA = {
    "Base": 75,         # Rich die-cast lacquer reflection
    "Accent": 60,       # Glossy decal shine
    "Grey metal": 115,  # High-specular mirror chrome reflection
    "Rims": 115,        # Chrome sheen
    "Window": 85,       # Canopy glass reflection
}

def mat_mul_4x4(A, B):
    C = [0.0]*16
    for col in range(4):
        for row in range(4):
            val = 0.0
            for k in range(4):
                val += A[k*4 + row] * B[col*4 + k]
            C[col*4 + row] = val
    return C

def mul_mat_vec(m, v):
    x = m[0]*v[0] + m[4]*v[1] + m[8]*v[2] + m[12]
    y = m[1]*v[0] + m[5]*v[1] + m[9]*v[2] + m[13]
    z = m[2]*v[0] + m[6]*v[1] + m[10]*v[2] + m[14]
    return (x, y, z)

def to_osg(p):
    """Invert the glTF rotation (x, z, -y) back to the OSG scene graph coords."""
    return (p[0], -p[2], p[1])

def load_octane_body(octane_dir: Path) -> tuple[dict, list]:
    gltf = json.loads((octane_dir / "model.gltf").read_text())
    osg = json.loads((octane_dir / "file.osgjs").read_text())
    bin_data = (octane_dir / "model.bin").read_bytes()

    accessors = gltf["accessors"]
    buffer_views = gltf["bufferViews"]

    def get_accessor(acc_idx: int):
        acc = accessors[acc_idx]
        bv = buffer_views[acc["bufferView"]]
        offset = bv.get("byteOffset", 0) + acc.get("byteOffset", 0)
        count = acc["count"]
        comp_map = {5126: ("f", 4), 5123: ("H", 2), 5125: ("I", 4), 5121: ("B", 1)}
        fmt_char, size = comp_map[acc["componentType"]]
        num_comp = {"SCALAR": 1, "VEC2": 2, "VEC3": 3, "VEC4": 4}[acc["type"]]
        stride = bv.get("byteStride", size * num_comp)
        res = []
        for i in range(count):
            vals = struct.unpack_from("<" + fmt_char * num_comp, bin_data, offset + i * stride)
            res.append(vals if num_comp > 1 else vals[0])
        return res

    top_mtx = osg["osg.Node"]["Children"][0]["osg.MatrixTransform"]["Matrix"]
    root_mt = osg["osg.Node"]["Children"][0]["osg.MatrixTransform"]["Children"][0]["osg.MatrixTransform"]

    geom_to_mworld = {}
    for child in root_mt["Children"]:
        mt = child.get("osg.MatrixTransform")
        if not mt:
            continue
        part_name = mt.get("Name")
        matrix = mt.get("Matrix")
        M_world = mat_mul_4x4(top_mtx, matrix)
        for sub in mt.get("Children", []):
            for g_item in sub.get("osg.Node", {}).get("Children", []):
                if "osg.Geometry" in g_item:
                    geom_to_mworld[g_item["osg.Geometry"].get("Name")] = (part_name, M_world)

    # Scale S matches Twin Mill's 120.303-inch wheelbase exactly
    S = 120.303 / 290.29
    def to_game(p):
        xg = p[0] * S
        yg = (148.78 - p[1]) * S
        zg = p[2] * S
        return [round(xg, 3), round(yg, 3), round(zg, 3)]

    # Skip original low-poly torus wheels; we replace them with authentic HW 5-spoke wheels
    wheel_part_names = {"Torus.001", "Torus.002", "Torus.003", "Torus.004", "Torus.005",
                        "Torus.006", "Torus.007", "Torus.008", "Torus.009", "Torus.010", "Torus.011"}

    materials = gltf.get("materials", [])
    body_data = {"positions": [], "uvs": [], "indices": [], "materials": []}

    for node in gltf["nodes"][:-1]:
        name = node.get("name")
        if not name or name not in geom_to_mworld:
            continue
        part_name, M_world = geom_to_mworld[name]
        if part_name in wheel_part_names:
            continue
        mesh = gltf["meshes"][node["mesh"]]
        for prim in mesh["primitives"]:
            pos_raw = get_accessor(prim["attributes"]["POSITION"])
            uv_raw = get_accessor(prim["attributes"]["TEXCOORD_0"]) if "TEXCOORD_0" in prim["attributes"] else [(0.0, 0.0)] * len(pos_raw)
            idx_raw = get_accessor(prim["indices"])
            mat_idx = prim.get("material", 0)
            mat_name = materials[mat_idx].get("name", "Material") if mat_idx < len(materials) else "Material"
            
            game_pos = [to_game(mul_mat_vec(M_world, to_osg(p))) for p in pos_raw]
            base_idx = len(body_data["positions"])
            body_data["positions"].extend(game_pos)
            body_data["uvs"].extend(uv_raw)
            
            # Winding reversed due to Y reflection: [i0, i2, i1]
            for i in range(0, len(idx_raw), 3):
                tri = [base_idx + idx_raw[i], base_idx + idx_raw[i+2], base_idx + idx_raw[i+1]]
                body_data["indices"].append(tri)
                body_data["materials"].append(mat_name)

    # Build primary body groups
    body_groups = []
    groups_dict = {}
    for tri, mname in zip(body_data["indices"], body_data["materials"]):
        color = HW_COLORS.get(mname, [150, 150, 150, 255])
        key = ("solid", mname, tuple(color))
        g = groups_dict.setdefault(key, {"texture": None, "color": color, "mode": 0, "combine": 0, "pos": [], "uv": [], "idx": [], "map": {}})
        for v_idx in tri:
            uv = body_data["uvs"][v_idx]
            k = (v_idx, round(uv[0], 4), round(uv[1], 4))
            if k not in g["map"]:
                g["map"][k] = len(g["pos"]) // 3
                g["pos"].extend([round(c, 2) for c in body_data["positions"][v_idx]])
                g["uv"].extend([round(uv[0], 4), round(uv[1], 4)])
            g["idx"].append(g["map"][k])

    for g in groups_dict.values():
        del g["map"]
        body_groups.append(g)

    # Build die-cast metallic reflection passes (combine 4 with ENVMAP1, texture 0)
    # Applied to Base, Accent, Grey metal (chrome), and Window
    for mname, alpha in ENVMAP_ALPHA.items():
        refl_tris = [tri for tri, m in zip(body_data["indices"], body_data["materials"]) if m == mname]
        if not refl_tris:
            continue
        base_col = HW_COLORS.get(mname, [255, 255, 255, 255])
        refl_col = [base_col[0], base_col[1], base_col[2], alpha]
        g = {"texture": 0, "color": refl_col, "mode": 1, "combine": 4, "pos": [], "uv": [], "idx": [], "map": {}}
        for tri in refl_tris:
            for v_idx in tri:
                uv = body_data["uvs"][v_idx]
                k = (v_idx, round(uv[0], 4), round(uv[1], 4))
                if k not in g["map"]:
                    g["map"][k] = len(g["pos"]) // 3
                    g["pos"].extend([round(c, 2) for c in body_data["positions"][v_idx]])
                    g["uv"].extend([round(uv[0], 4), round(uv[1], 4)])
                g["idx"].append(g["map"][k])
        del g["map"]
        body_groups.append(g)

    all_bpos = body_data["positions"]
    bbox_min = [round(min(p[k] for p in all_bpos), 3) for k in range(3)]
    bbox_max = [round(max(p[k] for p in all_bpos), 3) for k in range(3)]

    return {"groups": body_groups, "bbox_min": bbox_min, "bbox_max": bbox_max}

def build_hot_wheels_octane(octane_dir: Path, tw_path: Path) -> dict:
    body_res = load_octane_body(octane_dir)
    tw = car_entry(tw_path)

    # Textures setup:
    # Tex 0: ENVMAP1 (injected by viewer)
    # Tex 1: Hot Wheels 5-Spoke Chrome Rim (Twin Mill tex 6)
    # Tex 2: Hot Wheels Textured Tyre Tread (Twin Mill tex 13)
    # Tex 3: Brake Lights (Twin Mill tex 7)
    textures = [None, tw["textures"][6], tw["textures"][13], tw["textures"][7]]

    # Re-map texture indices for Twin Mill wheels
    def adapt_wheel_groups(orig_groups, mirror_x: bool = False, scale_factor: float = 1.0):
        out = []
        for g in orig_groups:
            # Map Twin Mill tex 6 -> 1 (rim), 13 -> 2 (tread)
            new_tex = 1 if g["texture"] == 6 else (2 if g["texture"] == 13 else g["texture"])
            new_g = {
                "texture": new_tex,
                "color": g.get("color"),
                "mode": g.get("mode", 0),
                "combine": g.get("combine", 1),
                "pos": [],
                "uv": list(g["uv"]),
                "idx": [],
            }
            # Scale & mirror
            sx = -scale_factor if mirror_x else scale_factor
            sy = sz = scale_factor
            for i in range(0, len(g["pos"]), 3):
                new_g["pos"].extend([
                    round(g["pos"][i] * sx, 2),
                    round(g["pos"][i+1] * sy, 2),
                    round(g["pos"][i+2] * sz, 2),
                ])
            # Reverse winding if mirrored along X
            if mirror_x:
                for i in range(0, len(g["idx"]), 3):
                    new_g["idx"].extend([g["idx"][i], g["idx"][i+2], g["idx"][i+1]])
            else:
                new_g["idx"] = list(g["idx"])
            out.append(new_g)
        return out

    tw_front_wheel = tw["lods"][0]["wheels"][0]["groups"]
    tw_rear_wheel = tw["lods"][0]["wheels"][2]["groups"]

    # Octane wheel centers in game space
    # FL: [-39.5, 120.3, 14.3], FR: [+39.0, 120.3, 14.3]
    # RL: [-44.0, 0.0, 16.6], RR: [+44.0, 0.0, 16.6]
    wheels_data = [
        {
            "part": 1,
            "pos": [-39.508, 120.303, 14.316],
            "radius": 14.814,
            "groups": adapt_wheel_groups(tw_front_wheel, mirror_x=False, scale_factor=1.0),
        },
        {
            "part": 2,
            "pos": [39.011, 120.303, 14.316],
            "radius": 14.814,
            "groups": adapt_wheel_groups(tw_front_wheel, mirror_x=True, scale_factor=1.0),
        },
        {
            "part": 3,
            "pos": [-43.962, 0.0, 16.608],
            "radius": 17.962,
            "groups": adapt_wheel_groups(tw_rear_wheel, mirror_x=False, scale_factor=1.0),
        },
        {
            "part": 4,
            "pos": [43.962, 0.0, 16.608],
            "radius": 17.962,
            "groups": adapt_wheel_groups(tw_rear_wheel, mirror_x=True, scale_factor=1.0),
        },
    ]

    car = {
        "name": "OCTANE",
        "bbox_min": body_res["bbox_min"],
        "bbox_max": body_res["bbox_max"],
        "wheels": [
            {"part": w["part"], "pos": w["pos"], "radius": w["radius"]}
            for w in wheels_data
        ],
        "textures": textures,
        "lods": [
            {
                "part": 0,
                "body": body_res["groups"],
                "wheels": wheels_data,
            }
        ],
        # Twin Mill physics, handling, stats & audio
        "cwh": tw["cwh"],
        "engine_type": tw["engine_type"],
        "game_stats": tw["game_stats"],
        "axles": tw["axles"],
        "handling": tw["handling"],
        # Headlight cones
        "light_fans": [
            [[22, 145, 18, 220, 220, 220, 130], [-180, 1310, 20, 200, 200, 200, 56], [220, 1310, 20, 200, 200, 200, 56], [-270, 1106, -69, 200, 200, 200, 8], [310, 1106, -69, 200, 200, 200, 8]],
            [[-22, 145, 18, 220, 220, 220, 130], [-220, 1310, 20, 200, 200, 200, 56], [179, 1310, 20, 200, 200, 200, 56], [-310, 1106, -69, 200, 200, 200, 8], [269, 1106, -69, 200, 200, 200, 8]],
        ],
        "brake_lights": {"texture": 3, "off": tw["brake_lights"]["off"] if tw.get("brake_lights") else None},
        "light_strips": tw.get("light_strips", []),
    }
    return car

if __name__ == "__main__":
    octane_dir = ROOT / "assets/workbench/custom_cars/octane/octane"
    tw_path = ROOT / "assets/files/TWINMILL.CAR"
    out_path = ROOT / "assets/workbench/custom_cars/OCTANE.car.json"
    out_path.parent.mkdir(parents=True, exist_ok=True)

    car = build_hot_wheels_octane(octane_dir, tw_path)
    out_path.write_text(json.dumps(car, separators=(",", ":")))
    print(f"Generated Hot Wheels styled {out_path} ({len(out_path.read_bytes())} bytes)")
    print(f"BBox: min={car['bbox_min']} max={car['bbox_max']}")
    print(f"Wheels: {car['wheels']}")
    print(f"Body groups: {len(car['lods'][0]['body'])} (including {sum(1 for g in car['lods'][0]['body'] if g['combine'] == 4)} die-cast reflection passes)")
    print(f"Textures count: {len(car['textures'])}")
