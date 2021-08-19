#!/bin/bash

torus_build_sh_on_close() {
  cd "$current_dir"
}


torus_build_sh() {
  current_dir="$PWD"

  trap "torus_build_sh_on_close $@" INT TERM

  mkdir -p build

  cd build

  cmake ..

  make

  torus_build_sh_on_close $@
}

torus_build_sh $@
