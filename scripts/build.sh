#!/bin/bash

pwd=$(pwd)

echo "==> BUILDING <=="
echo ""

cd ./build/
cmake .. && make

echo ""
echo "==> BUILT <=="

cd $pwd