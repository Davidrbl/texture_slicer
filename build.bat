@echo off
set flags=-Wall -g
set INCLUDE_FOLDER=./include
mkdir build\src
mkdir build\libs
@echo on

gcc -c src/main.c %flags% -I %INCLUDE_FOLDER% -o build/src/main.c.o
gcc -c src/shader.c %flags% -I %INCLUDE_FOLDER% -o build/src/shader.c.o
gcc -c src/bmp_imp.c %flags% -I %INCLUDE_FOLDER% -o build/src/bmp_imp.c.o
gcc -c src/texture_slicing.c %flags% -I %INCLUDE_FOLDER% -o build/src/texture_slicing.c.o
gcc -c libs/gl.c %flags% -I %INCLUDE_FOLDER% -o build/libs/gl.c.o

gcc ^
build/src/main.c.o ^
build/src/shader.c.o ^
build/src/bmp_imp.c.o ^
build/src/texture_slicing.c.o ^
build/libs/gl.c.o ^
-lglfw3 ^
-o main.exe
