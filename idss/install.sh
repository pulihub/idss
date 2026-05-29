#! /bin/bash

rm -fr /idss/build;

cmake -S /idss -B /idss/build -DCMAKE_BUILD_TYPE="Release" -D CMAKE_C_COMPILER=clang -D CMAKE_CXX_COMPILER=clang++; 
cmake --build /idss/build -v;
cmake --install /idss/build -v;
