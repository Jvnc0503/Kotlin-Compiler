#!/usr/bin/env bash
set -euo pipefail

# ─────────────────────────── VALIDACIÓN ───────────────────────────
if [[ $# -ne 1 ]]; then
  echo "Uso: $0 <archivo_de_entrada>"
  exit 1
fi

INPUT_FILE=$1
[[ -f $INPUT_FILE ]] || { echo "El archivo '$INPUT_FILE' no existe."; exit 1; }

# ────────────────────────── RUTAS BÁSICAS ─────────────────────────
ROOT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
SRC_DIR="$ROOT_DIR/src"
BIN_DIR="$ROOT_DIR/ejecutables"

mkdir -p "$BIN_DIR"

# ──────────────────────── COMPILAR EL COMPILADOR ──────────────────
echo "Compilando programa C++..."
g++ -std=c++20 \
    -I"$SRC_DIR" \
    $(find "$SRC_DIR" -name '*.cpp') \
    -o "$BIN_DIR/x"

echo "Compilación exitosa"

# ───────────────────── EJECUTAR EL COMPILADOR ─────────────────────
echo -e "\nEjecutando el archivo de entrada: $INPUT_FILE"
pushd "$BIN_DIR" >/dev/null
./x "$ROOT_DIR/$INPUT_FILE"
popd >/dev/null

# ───────────────────────── ENSAMBLAR output.s ─────────────────────
ASM_FILE="$BIN_DIR/output.s"
[[ -f $ASM_FILE ]] || { echo "No se encontró $ASM_FILE"; exit 1; }

echo -e "\nCompilando output.s con gcc..."
gcc "$ASM_FILE" -o "$BIN_DIR/output_exec"

echo -e "\nEjecutando programa generado:"
"$BIN_DIR/output_exec"
