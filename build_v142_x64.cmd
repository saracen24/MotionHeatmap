@echo off
setlocal

:: Check input arguments.
if not "%1" == "Debug" if not "%1" == "Release" (
  echo "Usage:"
  echo "%~nx0 <Debug|Release>"
  goto eof
)

set BUILD_TYPE=%1

:: Set MSBuild environment variables.
set VSWHERE="%ProgramFiles(x86)%/Microsoft Visual Studio/Installer/vswhere.exe"
set VSWHEREOPT=-version ^^^[16.0^^^,17.0^^^) -requires Microsoft.Component.MSBuild -property installationPath
for /f "usebackq tokens=*" %%i in (`%VSWHERE% %VSWHEREOPT%`) do (
  if exist "%%i" (
    set VS2019INSTALLDIR=%%i
  )
)

:: Setup developer command prompt for Visual Studio.
call "%VS2019INSTALLDIR%/Common7/Tools/VsDevCmd.bat" -arch=x64

:: Build.
cmake -S "." -B "build/%BUILD_TYPE%" -G "Visual Studio 16 2019" -A x64 -T host=x64 ^
  -DCMAKE_BUILD_TYPE:STRING=%BUILD_TYPE% ^
  -DBUILD_SHARED_LIBS:BOOL=ON ^
  -DBUILD_TESTS:BOOL=OFF ^
  -DCMAKE_INSTALL_PREFIX:PATH="%cd%/build/%BUILD_TYPE%/install" ^
  -DOpenCV_DIR:PATH=""
cmake --build "build/%BUILD_TYPE%" --config %BUILD_TYPE% --parallel 4
cmake --install "build/%BUILD_TYPE%" --config %BUILD_TYPE% --strip

:: Pack.
cd "build/%BUILD_TYPE%"
"%ProgramFiles%/CMake/bin/cpack.exe" -G ZIP -C %BUILD_TYPE%

:eof
endlocal
pause
