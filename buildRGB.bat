@echo off
del "VisionTest.exe"

clang++ vision.cpp -o VisionTest C:\Mingw\lib\libgdi32.a -O2
echo done compiling
"VisionTest"
popd
