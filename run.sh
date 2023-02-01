#!/bin/sh

if [ -z "$1" ]; then
	echo "usage: run.sh ASMTCODE [-d]"
	exit 1
fi

ASMT="$1"
if [ "$2" = "-d" ]; then
	DEBUG_OPT="-x"
else
	DEBUG_OPT=""
fi

SUBDIR="$PWD"
BASEDIR=$(realpath $(dirname "$0"))

cd "$BASEDIR"
docker run -it --rm -v "$SUBDIR:/submission_src" -v "$BASEDIR:/tests" bjucps/cps360-test /bin/bash $DEBUG_OPT "/tests/rundocker.sh" "$ASMT"
