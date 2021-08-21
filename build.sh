#!/bin/bash

torus_build_sh_on_close() {
  cd "$current_dir"
}


torus_build_sh() {
  current_dir="$PWD"

  trap "torus_build_sh_on_close $@" INT TERM

  rm -rf build
  rm -rf torus

  mkdir -p build

  cd build

  rm torus 2>/dev/null

  cmake $@ ..

  make

  if [[ "$@" =~ ^.*[[:space:]]*-DCMAKE_BUILD_TYPE=Release[[:space:]]*.*$ ]]; then
    strip -s torus
  fi

  cd ..

  cp build/torus .

  torus_build_sh_on_close $@
}

torus_build_sh $@
