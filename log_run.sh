#!/bin/bash

# Unusable for now, some color information remains in the log
# unbuffer ./run.sh $@ |& tee run.log
# cat run.log | sed 's/\x1B[@A-Z\\\]^_]\|\x1B\[[0-9:;<=>?]*[-!"#$%&'"'"'()*+,.\/]*[][\\@A-Z^_`a-z{|}~]//g' | tee run.log 

touch run.log
truncate -s 0 run.log
# For now forget about colors and just pipe to file
./run.sh $@ |& tee run.log
