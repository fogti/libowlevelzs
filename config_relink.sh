#!/bin/sh

[ -z "$1" ] || [ -z "$2" ] || [ -z "$3" ] && exit 1
CMAKE="$1" GENDIR="$2" FILEN="$3"
mkdir -p "$GENDIR/zs/ll"
cd "$GENDIR/zs/ll" || exit 1
rm -f "$3"
exec "$CMAKE" -E create_symlink "../../$3" "$3"
