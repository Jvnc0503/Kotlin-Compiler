#!/usr/bin/env python3
import subprocess as sp
from pathlib import Path
import re
import sys

# ──────────────────────── Rutas base ──────────────────────────────
ROOT_DIR = Path(__file__).resolve().parent
SRC_DIR  = ROOT_DIR / "src"
BIN_DIR  = ROOT_DIR / "ejecutables"
IN_DIR   = ROOT_DIR / "Inputs"
OUT_DIR  = ROOT_DIR / "Outputs"

BIN_DIR.mkdir(exist_ok=True)
OUT_DIR.mkdir(exist_ok=True)

# ────────────────── Compilar el compilador (una vez) ──────────────
print("Compilando programa C++ …")
cpp_files = [str(p) for p in SRC_DIR.rglob("*.cpp")]
exe_compiler = BIN_DIR / "x"

sp.run(
    ["g++", "-std=c++20", "-I", str(SRC_DIR), *cpp_files, "-o", str(exe_compiler)],
    check=True,
)
print(f"Compilador listo   →   {exe_compiler}\n")

# ───────────────────────  Helper: n º de input  ───────────────────
num_re = re.compile(r"input(\d+)\.kt$", re.IGNORECASE)

def index_of(kt_path: Path) -> str:
    """Devuelve el número dentro de 'input<N>.kt' como string; si no hay número,
    usa el nombre completo sin extensión."""
    m = num_re.match(kt_path.name)
    return m.group(1) if m else kt_path.stem  # p.ej. 'inputX'

# ──────────────────── Procesar todos los inputs ───────────────────
kt_files = sorted(IN_DIR.glob("*.kt"))
if not kt_files:
    print(f"No se encontraron archivos .kt en {IN_DIR}")
    sys.exit(0)

for kt in kt_files:
    idx = index_of(kt)                      # "3" para input3.kt
    asm_tmp  = BIN_DIR / "output.s"         # lo genera tu compilador
    asm_dest = OUT_DIR / f"output{idx}.s"   # queremos conservarlo
    exe_dest = BIN_DIR / f"output{idx}_exec"

    print("────────────────────────────────────────────────────────")
    print(f"→ Compilando   {kt.name}")

    # 1) Ejecutar compilador ⇒ output.s
    sp.run([str(exe_compiler), str(kt.resolve())], check=True)

    if not asm_tmp.exists():
        print(f"❌ No se generó {asm_tmp}")
        sys.exit(1)

    asm_tmp.replace(asm_dest)   # mueve/renombra

    # 2) Ensamblar y enlazar
    sp.run(["gcc", str(asm_dest), "-o", str(exe_dest)], check=True)

    # 3) Ejecutar resultado
    print(f"→ Ejecutando    {exe_dest.name}")
    sp.run([str(exe_dest)], check=True)
    print()

print("══════════════════  FIN DE TODOS LOS INPUTS  ══════════════════")
