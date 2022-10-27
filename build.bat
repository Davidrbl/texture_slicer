@echo off
set flags=-Wall -g
set INCLUDE_FOLDER=./include
@echo on

gcc -c ./src/main.c %flags% -I %INCLUDE_FOLDER% -o obj/main.o
gcc -c ./src/shader.c %flags% -I %INCLUDE_FOLDER% -o obj/shader.o
gcc -c ./src/gl.c %flags% -I %INCLUDE_FOLDER% -o obj/gl.o
gcc -c ./src/bmp_imp.c %flags% -I %INCLUDE_FOLDER% -o obj/bmp_imp.o
gcc -c ./src/texture_slicing.c %flags% -I %INCLUDE_FOLDER% -o obj/texture_slicing.o

gcc ^
obj/main.o ^
obj/shader.o ^
obj/gl.o ^
obj/bmp_imp.o ^
obj/texture_slicing.o ^
-lglfw3 ^
-o main.exe
