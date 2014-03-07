#!/bin/bash

mydir="$(dirname "$0")"
(
	cd "$mydir"
	make clean
	PKG_CONFIG_PATH=/opt/X11/lib/pkgconfig make
	if [[ $? ]]; then
		mv a.out cgos_mac
	fi
)