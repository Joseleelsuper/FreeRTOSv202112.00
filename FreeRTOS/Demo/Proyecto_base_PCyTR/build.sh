#!/bin/bash

make clean 2> ./error.txt

make PROFILE=1 2> ./error.txt
if [ $? -ne 0 ]; then
    echo "Error en el paso: make PROFILE=1. Revisa ./error.txt para más detalles."
    exit 1
fi

make profile 2> ./error.txt
if [ $? -ne 0 ]; then
    echo "Error en el paso: make profile. Revisa ./error.txt para más detalles."
    exit 1
fi

./build/posix_demo 2> ./error.txt
if [ $? -ne 0 ]; then
    echo "Error en el paso: ./build/posix_demo. Revisa ./error.txt para más detalles."
    exit 1
fi