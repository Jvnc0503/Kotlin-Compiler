#!/bin/bash

# Verificar argumento
if [ $# -ne 1 ]; then
  echo "Uso: $0 <archivo_de_entrada>"
  exit 1
fi

INPUT_FILE="$1"

if [ ! -f "$INPUT_FILE" ]; then
  echo "El archivo '$INPUT_FILE' no existe."
  exit 1
fi

# Entrar al directorio src/
cd src || { echo "No se encontró el directorio src/"; exit 1; }

echo "Compilando programa C++..."
g++ main.cpp exp.cpp parser.cpp scanner.cpp token.cpp visitor.cpp -o x
if [ $? -ne 0 ]; then
  echo "Fallo la compilación del programa."
  exit 1
fi

echo "Ejecutando parser con archivo de entrada: $INPUT_FILE"
./x "../$INPUT_FILE"
if [ $? -ne 0 ]; then
  echo "Fallo la ejecución del parser."
  exit 1
fi

# Compilar el output.s generado
ASM_FILE="output.s"
if [ ! -f "$ASM_FILE" ]; then
  echo "No se encontró el archivo ensamblador $ASM_FILE"
  exit 1
fi

echo "Compilando ensamblador con gcc..."
gcc "$ASM_FILE" -o output_exec
if [ $? -ne 0 ]; then
  echo "Fallo la compilación del ensamblador."
  exit 1
fi

echo "Ejecutando programa generado:"
./output_exec
