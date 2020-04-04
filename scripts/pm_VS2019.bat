@echo off
pushd %~dp0\..\
CALL vendor\premake\premake5.exe vs2019
popd
PAUSE