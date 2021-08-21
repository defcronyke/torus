@echo off

mkdir build

cd build

cmake ..

msbuild ALL_BUILD.vcxproj /property:Configuration=Release

cd ..

cp build/Release/torus.exe .
