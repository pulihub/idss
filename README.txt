******************************************

IDSS - The InnoCYPES Data Storage Service

This software Copyright (C) 2022-2026
University of Salento, Italy

Design and implementation by:

Prof. Massimo Cafaro
Prof. Italo Epicoco

CMake configuration and Docker image:
Prof. Marco Pulimeno

******************************************


This package includes a web service and related sample clients.

To build, first install the dependencies listed in dependencies.txt, then:
cmake -S . -B build -DCMAKE_BUILD_TYPE="Release" -D CMAKE_C_COMPILER=clang -D CMAKE_CXX_COMPILER=clang++ [-DCMAKE_INSTALL_PREFIX="installation directory"];

cmake --build build -v;

NOTE: Use -DCMAKE_INSTALL_PREFIX if you want to change the installation directory, which is by default /usr/local

To install (default is /usr/local):
cmake --install build -v;

To clean, simply remove the build directory:
rm -fr build

******************************************

gsoap-2.8 folder is the gSOAP toolkit source code, licensed under GPLv2 from GENIVIA: https://www.genivia.com
