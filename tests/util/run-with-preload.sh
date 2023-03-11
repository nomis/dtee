#!/bin/bash

. "$(dirname -- "$0")"/preload.sh

cd tests || exit 1

preload_setup
"$@"
preload_cleanup
