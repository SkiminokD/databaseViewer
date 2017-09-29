@echo off
rem -----------------------------------------------
rem Usages:
rem fill-db.cmd [path-to-pg-bin]\ [port] [pglogin] [pgpassword]
rem -----------------------------------------------
IF [%4] == [] (
    echo "Usages: fill-db.cmd [path-to-pg-bin]\ [port] [pglogin] [pgpassword]"
) else (
	set PGPATH=%1
	set PGPASSWORD=%4
	echo "Work in progress. Please wait..."
	%PGPATH%\psql.exe --host=localhost --port=%2 --username=%3 --file=data.sql "inobitec"
)