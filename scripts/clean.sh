#!/bin/bash

pwd=$(pwd)

echo "==> CLEANING <=="
echo ""

rm -rf ./build/*
rm -rf ./cmake-build-debug/*

cd $pwd