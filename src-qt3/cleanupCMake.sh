#!/bin/bash

find . -name CMakeFiles -type d -exec rm -r {} \;
find . -name CMakeCache.txt -exec rm {} \;
find . -name cmake_install.cmake -exec rm {} \;
find . -name install_manifest.txt -exec rm {} \;
rm CPackSourceConfig.cmake
rm CPackConfig.cmake
rm -r _CPack_Packages/