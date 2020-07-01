#!/bin/bash
#
# Copyright (c) 2020 Andrew Smith
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

script_name="generate_data.sh"

print_info () {
  green="\e[32m"
  nc="\e[0m"
  echo -e "$green[ $script_name ]$nc $1"
}

print_info "#########################################################"
print_info "#                                                       #"
print_info "#               Generating Test data                    #"
print_info "#                                                       #"
print_info "#########################################################"

#State these before running the script
export NUM_EVENTS=256
export NUM_ACTIONS=2

export ROOT_DIR="$PWD"
export RAW_DATA_DIR="/scratch/predict-T/training_data/training_data_1"
export CLEAN_DATA_DIR="$ROOT_DIR/cleaned_data_${NUM_EVENTS}EVENTS_${NUM_ACTIONS}ACTIONS"


print_info "export ROOT_DIR $ROOT_DIR"
print_info "export RAW_DATA_DIR $RAW_DATA_DIR"
print_info "export CLEAN_DATA_DIR $CLEAN_DATA_DIR"

export FILE_NAMES=""
for files in "$RAW_DATA_DIR"/*
do
  FILE_NAMES="$FILE_NAMES$files"
  FILE_NAMES="${FILE_NAMES},"
done

if [ ! -d $CLEAN_DATA_DIR ]; then
  print_info "creating $CLEAN_DATA_DIR"
  mkdir -p $CLEAN_DATA_DIR
else
  rm -rf $CLEAN_DATA_DIR/*
fi

echo ""

./training_data.py \
  --input=$FILE_NAMES \
  --output=$CLEAN_DATA_DIR \
  --events=$NUM_EVENTS \
  --actions=$NUM_ACTIONS \
  --threads=16
#  --pc=False
