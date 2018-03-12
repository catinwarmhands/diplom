@echo off

IF "%~1"=="" (
	tcc -run main.c
) ELSE IF "%~1" == "release" (
	tcc main.c
) ELSE (
	echo Error: unknown command "%1"
)