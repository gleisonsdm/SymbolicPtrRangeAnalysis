#!/bin/bash

#You can use this script to run DawnCC, passing the DawnCC root dir, arguments to change output and files to be processed
#./run.sh -d (DawnCC root dir - containing DawnCC and llvm-build) -src (folder with *.c and *.cpp files to be processed)


#Set default parameters of DawnCC
CURRENT_DIR=`pwd`
DEFAULT_ROOT_DIR=`pwd`
FILES=""
DBGPRINT=""
LLVM_PATH=""

#Process arguments of script
while [ $# -gt 1 ]
do
    key="$1"

    case $key in
        -d|--DebugPRinter)
            DBGPRINT="$2" #if you pass the DawnCC root dir (folder containing llvm-build and DawnCC, it's not going to assume that you're currently on it)
            shift # past argument
        ;;
        -llvm|--LLVMDir)
            LLVM_PATH="$2" # path to be scanned and have files processed
            shift
        ;;
        -f|--File)
            FILE="$2"
            shift
        ;;
        *)
            # unknown option
        ;;
    esac
    shift # past argument or value
done


#Export path to llvm and its tools
export CLANG="${LLVM_PATH}/bin/clang"
export CLANGFORM="${LLVM_PATH}/bin/clang-format"
export OPT="${LLVM_PATH}/bin/opt"

#Export path to PtrRA libraries
export PTRRA="${DBGPRINT}/libLLVMPTRRA.so"

#Export tools flags
export FLAGS=" -mem2reg -instnamer -basicaa "

#Temporary files names
TEMP_FILE1="result.bc"
TEMP_FILE2="result2.bc"
LOG_FILE="out_pl.log"

$CLANG -g -S -emit-llvm ${FILE} -o ${TEMP_FILE1} 

$OPT -load $PTRRA $FLAGS -print-mem-dep -dump-dbg-info -S ${TEMP_FILE1} -o ${TEMP_FILE2}

