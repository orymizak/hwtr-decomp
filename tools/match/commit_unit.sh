#!/bin/sh
# commit_unit.sh [--force] "<mensaje>" <archivos...>: confirma como mucho cada 5 minutos y solo si la ROM es
# idéntica (SHA-1) y cada función en C coincide en su dirección final.
# Cada 3 commits la compilación es completa (make clean && make); en los demás, incremental.
# Antes de los 5 minutos no hace nada: los cambios siguen pendientes para el siguiente commit.
# --force confirma ya lo pendiente (al terminar una tanda de trabajo).
set -e
force=0
if [ "$1" = "--force" ]; then
    force=1
    shift
fi
msg=$1
shift
if [ $force = 0 ]; then
    age=$(( $(date +%s) - $(git log -1 --format=%ct) ))
    if [ "$age" -lt 300 ]; then
        echo "pendiente: faltan $((300 - age)) s para el siguiente commit"
        exit 0
    fi
fi
# Commits desde la última compilación completa: los que no dicen "make clean" en su mensaje.
since=0
for c in $(git log -10 --format=%H); do
    if git log -1 --format=%B "$c" | grep -q "make clean && make"; then
        break
    fi
    since=$((since + 1))
done
if [ "$since" -ge 2 ]; then
    make clean >/dev/null
    how="tras make clean && make"
else
    how="con compilación incremental"
fi
make -j8 >/dev/null 2>&1 || { echo "make falló"; make 2>&1 | tail -20; exit 1; }
sha=$(shasum build/hotwheelsturbo.z64 | cut -d' ' -f1)
if [ "$sha" != "94913a07e49005ffd43dfdfa16ff5862bdb93748" ]; then
    echo "ROM distinta: $sha"
    exit 1
fi
check=$(python3 tools/match/verify_funcs.py) || { echo "$check"; exit 1; }
echo "$check"
git add -- "$@"
git commit -q -m "$msg

ROM idéntica $how (SHA-1 $sha).
$check"
git log --oneline -1
