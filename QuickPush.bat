@echo off

set /p words=Have a commit: 

git add --all
git commit -m "%words%"
git push origin main

echo.
set /p s="Press Enter to continue:"

@echo on