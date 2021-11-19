#!/bin/bash

pwd=$(pwd)

./scripts/clean.sh
./scripts/build.sh

cd $pwd