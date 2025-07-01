import subprocess as sp
from pathlib import Path
import re
import sys

ROOT_DIR = Path(__file__).resolve().parent
SRC_DIR  = ROOT_DIR / "src"
BIN_DIR  = ROOT_DIR / "ejecutables"
IN_DIR   = ROOT_DIR / "Inputs"
OUT_DIR  = ROOT_DIR / "Outputs"

BIN_DIR.mkdir(exist_ok=True)
OUT_DIR.mkdir(exist_ok=True)

print("Compilando programa C++ …")
cpp_files = [str(p) for p in SRC_DIR.rglob("*.cpp")]
exe_compiler = BIN_DIR / "x"

sp.run(
    ["g++", "-std=c++20", "-I", str(SRC_DIR), *cpp_files, "-o", str(exe_compiler)],
    check=True,
)
print(f"Compilador listo   →   {exe_compiler}\n")

num_re = re.compile(r"input(\d+)\.kt$", re.IGNORECASE)

def index_of(kt_path: Path) -> str:
    m = num_re.match(kt_path.name)
    return m.group(1) if m else kt_path.stem  

kt_files = sorted(IN_DIR.glob("*.kt"))
if not kt_files:
    print(f"No se encontraron archivos .kt en {IN_DIR}")
    sys.exit(0)

for kt in kt_files:
    idx       = index_of(kt)
    asm_tmp   = BIN_DIR / "output.s"
    asm_dest  = OUT_DIR / f"output{idx}.s"
    exe_dest  = BIN_DIR / f"output{idx}_exec"

    print("────────────────────────────────────────────────────────")
    print(f"→ Compilando   {kt.name}")

    sp.run(
        [str(exe_compiler), str(kt.resolve())],
        cwd=BIN_DIR        
    )

    if not asm_tmp.exists():
        print(f"❌ No se generó {asm_tmp}")
        sys.exit(1)

    asm_tmp.replace(asm_dest)

    sp.run(["gcc", str(asm_dest), "-o", str(exe_dest)])

    print(f"→ Ejecutando    {exe_dest.name}")
    sp.run([str(exe_dest)])
    print()



print("══════════════════  FIN DE TODOS LOS INPUTS  ══════════════════")
