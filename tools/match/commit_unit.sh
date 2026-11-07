#!/bin/sh
# commit_unit.sh "<mensaje>" <archivos...>: confirma solo si la ROM es idéntica tras make clean && make.
set -e
msg=$1
shift
make clean >/dev/null
make -j8 >/dev/null 2>&1 || { echo "make falló"; make 2>&1 | tail -20; exit 1; }
sha=$(shasum build/hotwheelsturbo.z64 | cut -d' ' -f1)
if [ "$sha" != "94913a07e49005ffd43dfdfa16ff5862bdb93748" ]; then
    echo "ROM distinta: $sha"
    exit 1
fi
git add -- "$@"
git commit -q -m "$msg

ROM idéntica tras make clean && make (SHA-1 $sha)."
git log --oneline -1
