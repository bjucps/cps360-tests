#!/bin/sh

if [ -z "$1" ]; then
	echo "usage: run.sh ASMTCODE [-d] [--privileged]"
	exit 1
fi

ASMT="$1"
shift

if [ "$1" = "-d" ]; then
	DEBUG_OPT="-x"
	shift
else
	DEBUG_OPT=""
fi

if [ "$1" = "--privileged" ]; then
	DOCKER_PRIV="--privileged"
	shift
else
	DOCKER_PRIV=""
fi

SUBDIR="$PWD"
BASEDIR=$(realpath $(dirname "$0"))

cd "$BASEDIR"
docker run -it --rm $DOCKER_PRIV -v "$SUBDIR:/submission_src" -v "$BASEDIR:/tests" bjucps/cps360-test /bin/bash $DEBUG_OPT "/tests/rundocker.sh" "$ASMT"
