#!/bin/bash

pwd=$(pwd)

./scripts/clean.sh
./scripts/conan_build.sh

cd $pwd