#!/usr/bin/env bash

. "$(dirname -- "$0")"/preload.sh

cd tests || exit 1

exec 3>/dev/null
BASH_XTRACEFD=3

preload_setup
"$@"
preload_cleanup
