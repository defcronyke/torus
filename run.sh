#!/bin/bash

torus_run_sh_on_close() {
  cd "$current_dir"
}

torus_run_sh() {
  current_dir="$PWD"

  trap "torus_run_sh_on_close $@" INT TERM

  ./build.sh -DCMAKE_BUILD_TYPE=Release $@

  ./torus
  res=$?

  torus_run_sh_on_close $@

  if [ $res -eq 139 ]; then
    reset
  fi

  return $res
}

torus_run_sh $@
