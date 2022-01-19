#!/bin/bash

pwd=$(pwd)

echo "==> INSTALLING DEPENDENCIES <=="
echo ""

cd ./build/
conan install ..

echo ""
echo "==> INSTALLING DEPENDENCIES <=="

cd $pwd
