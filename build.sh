#!/usr/bin/env bash
CC="gcc"
CXX="g++"

FLAGS="-Wall -Wextra -O2"
CFLAGS="$FLAGS -std=c11"
CXXFLAGS="$FLAGS -std=c++20"
LDFLAGS="-lstdc++"

$CXX $CXXFLAGS -c maptel.cc
$CC  $CFLAGS   -c maptel_test1.c
$CC  $LDFLAGS  -o out maptel.o maptel_test1.o
