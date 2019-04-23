#!/bin/bash

LLVM_DIR=$1

cd ..
rm -rf build
mkdir build
cd build

CXX=g++-5 cmake -DLLVM_DIR=${LLVM_DIR}/share/llvm/cmake ../src

make -j4 

cd -
