@echo off
color 0a
del /f /q *.ncb
del /f /q *.dsw
del /f /q *.opt
del /f /q *.plg
del /f /q *.dsp
rd/s/q debug
rd/s/q release
rd/s/q testFolder