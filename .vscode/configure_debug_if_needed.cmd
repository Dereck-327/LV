@echo off
setlocal
set "WS=%~1"
if "%WS%"=="" set "WS=%cd%"
set "BUILD_DIR=%WS%\build_debug"
set "NINJA_FILE=%BUILD_DIR%\build.ninja"

if exist "%NINJA_FILE%" (
  echo [CMake] Debug configure skipped: existing build.ninja found.
  exit /b 0
)

echo [CMake] Debug configure required: build.ninja not found.
call "%WS%\.vscode\cmake_with_vsenv.cmd" -S "%WS%" -B "%BUILD_DIR%" -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_MAP_IMPORTED_CONFIG_DEBUG=Release -DCMAKE_PREFIX_PATH=C:/Qt/6.11.0/msvc2022_64 -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_C_COMPILER_LAUNCHER= -DCMAKE_CXX_COMPILER_LAUNCHER=
exit /b %errorlevel%
