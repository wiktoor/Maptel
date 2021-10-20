#!/usr/bin/env bash
CC="gcc"
CXX="g++"

FLAGS="-Wall -Wextra -O2"
CFLAGS="$FLAGS -std=c11"
CXXFLAGS="$FLAGS -std=c++17"
LDFLAGS="-lstdc++"

$CXX $CXXFLAGS -c maptel.cc -o maptel.o
$CC  $CFLAGS   -c maptel_test1.c -o maptel_test1.o
$CXX $CXXFLAGS -c maptel_test2.cc -o maptel_test2.o
$CXX maptel_test1.o maptel.o -o maptel_test1
$CXX maptel_test2.o maptel.o -o maptel_test21
$CXX maptel.o maptel_test2.o -o maptel_test22
