#!/bin/bash

torus_build_sh_on_close() {
  cd "$current_dir"
}


torus_build_sh() {
  current_dir="$PWD"

  trap "torus_build_sh_on_close $@" INT TERM

  mkdir -p build


  rm torus 2>/dev/null
  rm torus.exe 2>/dev/null
  rm torus.tar.7z 2>/dev/null
  
  cd build

  rm torus 2>/dev/null
  rm torus.exe 2>/dev/null

  cmake -DCMAKE_TOOLCHAIN_FILE=${PWD}/../windows-cross-compile-x86_64-w64-mingw32.cmake $@ ..

  make

  if [[ "$@" =~ ^.*[[:space:]]*-DCMAKE_BUILD_TYPE=Release[[:space:]]*.*$ ]]; then
    x86_64-w64-mingw32-strip -s torus.exe
  fi

  cd ..

  cp build/torus.exe .
  cp deps/freeglut-mingw/freeglut/bin/x64/freeglut.dll .

  mkdir -p torus

  cp torus.exe torus/
  cp deps/freeglut-mingw/freeglut/bin/x64/freeglut.dll torus/
  cp LICENSE* torus/

  7z a torus.7z torus

  torus_build_sh_on_close $@
}

torus_build_sh $@
