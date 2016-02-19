mkdir dist
mkdir dist\\windows

cd Tools\\Scripts
call npm install .
echo Installing JSC built for Windows 10
call node setup.js -s 10.0 --no-color --no-progress-bars

cd build
call npm install .
echo Building for Windows 10
call node build.js -s 10.0 -m 14.0 -o WindowsStore-x86 -o WindowsStore-ARM
SET LEVEL=%ERRORLEVEL%
rmdir node_modules /Q /S
cd ..

echo Updating timestamp on fake junit report
copy /b junit_report.xml +,,

cd ..\\..
copy Tools\\Scripts\\junit_report.xml dist\\junit_report.xml
copy Tools\\Scripts\\junit_report.xml dist\\junit_report_10.xml

exit /B %LEVEL%
