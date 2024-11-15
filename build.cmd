@echo off
rem because we're lazy
if not exist bin (
    mkdir bin
)
cd bin
cmake ..
cmake --build .
cd Debug
VMTTool.exe
cd ..
cd ..