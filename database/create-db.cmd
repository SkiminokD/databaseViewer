@echo off
rem -----------------------------------------------
rem Usages:
rem create-db.cmd [path-to-pg-bin]\ [port] [pglogin] [pgpassword]
rem -----------------------------------------------
IF [%4] == [] (
    echo "Usages: create-db.cmd [path-to-pg-bin]\ [port] [pglogin] [pgpassword]"
) else (
    set PGPATH=%1
    set PGPASSWORD=%4
    %PGPATH%\createdb.exe --host=localhost --port=%2 --username=%3 "inobitec"
    %PGPATH%\psql.exe --host=localhost --port=%2 --username=%3 --file=schema.sql "inobitec"
)
