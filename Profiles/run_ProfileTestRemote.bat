echo off
echo In progress... (it can be long the first time)
echo.
%~dp0\..\UjpsMainApp\release\UJPS_console.exe %~dp0\ProfileTestRemote\release\ProfileTestRemote.dll
echo.
echo press a key to close this window
pause
