@echo off
del "BlockTest.exe"

clang++ blocks.cpp -o BlockTest C:\Mingw\lib\libgdi32.a -O2
echo done compiling
"BlockTest"
popd
