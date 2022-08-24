@echo off
cscript /nologo /e:jscript configure.js  "--with-extra-includes=E:\Opencv\build\install\include" "--with-extra-libs=E:\Opencv\build\install\x64\vc17\lib" "--with-opencv" %*
