#!/bin/bash

pwd=$(pwd)

echo "==> CONAN BUILDING <=="
echo ""

cd ./build/
conan build ..

echo ""
echo "==> CONAN BUILT <=="

cd $pwd
