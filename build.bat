@echo off

mkdir build

cd build

cmake ..

msbuild ALL_BUILD.vcxproj /property:Configuration=Release

cd ..

cp build/Release/torus.exe .

mkdir torus

cp torus.exe torus/
cp freeglut.dll torus/
cp LICENSE* torus/
