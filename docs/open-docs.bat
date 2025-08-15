@echo off
set DOC_PATH=%~dp0html\index.html
if exist "%DOC_PATH%" (
    start "" "%DOC_PATH%"
) else (
    echo Documentation not found at %DOC_PATH%
)