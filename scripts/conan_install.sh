#!/bin/bash

pwd=$(pwd)

# Clean first. This is required because of the conan source command
./scripts/clean.sh

echo "==> INSTALLING DEPENDENCIES <=="
echo ""

cd ./build/

# This requires the dependencies folder inside /build to be empty. When a cached dependency is found it will stop cloning dependencies.
conan source .. --source-folder dependencies
# Change the build type to -b if it prefers remotes. try to build everything first, then the result will be cahced
conan install .. --build missing

echo ""
echo "==> INSTALLING DEPENDENCIES <=="

cd $pwd
