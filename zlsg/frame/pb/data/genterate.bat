@echo OFF
set /p var="Are you want to genterate?(all or name)"
if "%VAR%"=="" GOTO ALL
if %VAR%==all GOTO ALL
protoc --cpp_out=./ ./%VAR%.proto
@echo %~f0
goto END
:ALL
@echo all
protoc --cpp_out=./ ./*.proto
:END