#!/usr/bin/env sh
SCCD_ROOT="`dirname \"$0\"`"
SCCD_ROOT="`( cd \"$SCCD_ROOT\" && pwd )`"

if [ "$1" = "c25519" ]
then
	if [ "$2" = "SRCS" ]
	then
		echo "$SCCD_ROOT/3rdparty/c25519/src/f25519.c $SCCD_ROOT/3rdparty/c25519/src/ed25519.c"
	elif [ "$2" = "CCFLAGS" ]
	then
		echo "-DSCCD_BACKEND_C25519 -I$SCCD_ROOT/3rdParty"
	fi
elif [ "$1" = "sccd" ]
then
	if [ "$2" = "SRCS" ]
	then
		echo "$SCCD_ROOT/sccd/core/ec.c $SCCD_ROOT/sccd/core/fp.c $SCCD_ROOT/sccd/core/util.c $SCCD_ROOT/sccd/ibc/vbnn_ibs.c $SCCD_ROOT/3rdparty/BLAKE2/ref/blake2s-ref.c"
	elif [ "$2" = "CCFLAGS" ]
	then
		echo "-I$SCCD_ROOT -I$SCCD_ROOT/3rdparty/BLAKE2/ref"
	fi
fi
