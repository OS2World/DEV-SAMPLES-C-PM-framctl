REM SET C_INCLUDE_PATH=%UNIXROOT%/usr/include;%C_INCLUDE_PATH%
REM SET INCLUDE=%UNIXROOT%\usr\include
REM SET EMXOMFLD_TYPE=WLINK
REM SET EMXOMFLD_LINKER=wl.exe
make 2>&1 |tee make.out
