@echo off

cmd /c call build.cmd vc12-x64 static mt
if errorlevel 1 goto :error

goto :eof

:error
exit /b 1
