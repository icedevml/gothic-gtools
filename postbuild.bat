@echo off
copy /Y bin\Debug\gothicm.dll "D:\Program Files\Piranha Bytes\Gothic\System\gothicm.dll"
D:
cd Program Files\Piranha Bytes\Gothic\System
start zspy
start gothicmod -zwindow -zlog:8,s