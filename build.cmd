rem because we're lazy
@echo off
if not exist bin (
    mkdir bin
)
cd bin
cmake ..
cmake --build .
cd ..