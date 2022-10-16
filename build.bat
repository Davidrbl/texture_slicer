@REM @echo off
set flags=-Wall
set INCLUDE_FOLDER=./include

gcc -c ./src/main.c %flags% -I %INCLUDE_FOLDER% -o main.o
gcc -c ./src/shader.c %flags% -I %INCLUDE_FOLDER% -o shader.o
gcc -c ./src/gl.c %flags% -I %INCLUDE_FOLDER% -o gl.o
gcc -c ./src/bmp_imp.c %flags% -I %INCLUDE_FOLDER% -o bmp_imp.o

gcc ^
main.o ^
shader.o ^
gl.o ^
bmp_imp.o ^
-lglfw3 ^
-o main.exe