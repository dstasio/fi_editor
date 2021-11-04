@echo off

if NOT DEFINED proj_root (
call "%~dp0\shell.bat"
)
pushd "%proj_root%"

IF NOT EXIST ".\build" ( mkdir ".\build")

SETLOCAL

pushd ".\build"

set      ignored_warnings=-wd4201 -wd4100 -wd4189 -wd4456 -wd4505
set common_compiler_flags=-diagnostics:column -MTd -nologo -Gm- -GR- -EHa- -Od -Oi -WX -W4 %ignored_warnings% -FAsc -Z7
set   common_linker_flags=-incremental:no -opt:ref user32.lib gdi32.lib

REM 64-bit build
del *.pdb > NUL 2> NUL
cl %common_compiler_flags% "%proj_root%\code\win32_layer.cpp" -Fmwin32_layer.map /link %common_linker_flags% d3d11.lib

popd REM .\build
popd REM %proj_root
ENDLOCAL
