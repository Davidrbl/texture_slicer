@echo off
set flags=-Wall
set INCLUDE_FOLDER=./include
@echo on

gcc -c ./src/main.c %flags% -I %INCLUDE_FOLDER% -o main.o
gcc -c ./src/shader.c %flags% -I %INCLUDE_FOLDER% -o shader.o
gcc -c ./src/gl.c %flags% -I %INCLUDE_FOLDER% -o gl.o
gcc -c ./src/bmp_imp.c %flags% -I %INCLUDE_FOLDER% -o bmp_imp.o
gcc -c ./src/texture_slicing.c %flags% -I %INCLUDE_FOLDER% -o texture_slicing.o

gcc ^
main.o ^
shader.o ^
gl.o ^
bmp_imp.o ^
texture_slicing.o ^
-lglfw3 ^
-o main.exe