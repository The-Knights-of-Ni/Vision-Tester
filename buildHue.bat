@echo off
del "HueTest.exe"

clang++ hue.cpp -o HueTest C:\Mingw\lib\libgdi32.a -O2
echo done compiling
"HueTest"
popd
