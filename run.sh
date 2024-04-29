#!/bin/sh

if [ -z "$1" ]; then
	echo "usage: run.sh ASMTCODE [-i] [-d] [--privileged]"
	exit 1
fi

ASMT="$1"
shift

if [ "$1" = "-i" ]; then
	ENTRY_SCRIPT="-i"
	shift
else
	ENTRY_SCRIPT="/tests/rundocker.sh $ASMT"
fi

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
docker run -it --rm $DOCKER_PRIV -v "$SUBDIR:/submission_src" -v "$BASEDIR:/tests" bjucps/cps360-test /bin/bash $DEBUG_OPT $ENTRY_SCRIPT
