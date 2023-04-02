@echo off

setlocal enabledelayedexpansion

for /f "tokens=2" %%i in ('tasklist /FI "IMAGENAME eq main.exe" /NH') do (
  taskkill /PID %%i /F
)
