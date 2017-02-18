#!/bin/bash

OUT_DIR=build

if [ -d "${OUT_DIR}" ]; then
    rm -rf ${OUT_DIR}
fi

mkdir ${OUT_DIR}
cd ${OUT_DIR}
cmake ..
make 
