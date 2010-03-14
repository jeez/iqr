@echo off
set XEENA_HOME=C:\ProgramFiles\Text\Xeena


set DTDFILE="Prc_Connections.dtd"


%XEENA_HOME%\xeena -dtd %DTDFILE%  -root "ConnectionTo"