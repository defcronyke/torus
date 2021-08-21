#!/bin/bash

torus_run_sh_on_close() {
  cd "$current_dir"
}

torus_run_sh() {
  current_dir="$PWD"

  trap "torus_run_sh_on_close $@" INT TERM

  ./build-mingw.sh -DCMAKE_BUILD_TYPE=Release $@

  wine torus.exe

  torus_run_sh_on_close $@
}

torus_run_sh $@
