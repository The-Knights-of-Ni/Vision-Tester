@echo off
del "VisionTest.exe"

clang++ vision.cpp -o VisionTest C:\Mingw\lib\libgdi32.a
echo done compiling
"VisionTest"
popd
