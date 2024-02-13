@echo off
cd out\build
if errorlevel 1 exit /b %errorlevel%
make curl_LDFLAGS=-all-static