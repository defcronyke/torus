@echo off

mkdir build

cd build

cmake .. 

make

cd ..

cp build/Release/torus.exe .
cp deps/freeglut-mingw/freeglut/bin/x64/freeglut.dll .

mkdir torus

cp torus.exe torus/
cp deps/freeglut-mingw/freeglut/bin/x64/freeglut.dll torus/
cp LICENSE* torus/
