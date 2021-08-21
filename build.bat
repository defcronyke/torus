@echo off

mkdir build

cd build

cmake ..

msbuild ALL_BUILD.vcxproj /property:Configuration=Release

cd ..

cp build/Release/torus.exe .
cp deps/freeglut-msvc/freeglut/bin/x64/freeglut.dll .

mkdir torus

cp torus.exe torus/
cp deps/freeglut-msvc/freeglut/bin/x64/freeglut.dll torus/
cp LICENSE* torus/
