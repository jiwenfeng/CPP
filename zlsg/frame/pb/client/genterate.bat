@echo OFF
set /p var="Are you want to genterate?(all or name)"
if "%VAR%"=="" GOTO ALL
if %VAR%==all GOTO ALL
protoc --cpp_out=./ ./%VAR%.proto
@copy %VAR%.pb.h ..\..\..\..\libpact\
@copy %VAR%.pb.cc ..\..\..\..\libpact\
@copy %VAR%.proto ..\..\..\..\libpact\
@echo %~f0
goto END
:ALL
@echo all
protoc --cpp_out=./ ./*.proto
@copy *.proto ..\..\..\..\libpact\
@copy *.pb.h ..\..\..\..\libpact\
@copy *.pb.cc ..\..\..\..\libpact\
:END
