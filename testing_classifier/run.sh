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

script_name="run.sh"

print_info () {
  green="\e[32m"
  nc="\e[0m"
  echo -e "$green[ $script_name ]$nc $1"
}

print_info "#########################################################"
print_info "#                                                       #"
print_info "#               Running Tests                           #"
print_info "#                                                       #"
print_info "#########################################################"

#State these before running the script
export NUM_EVENTS=50
export NUM_ACTIONS=6

export ROOT_DIR="$HOME/predict_t_classifier/testing_classifier"
export RAW_DATA_DIR="$HOME/predict_t_classifier/data-cleaning/raw_data"
export CLEAN_DATA_DIR="$ROOT_DIR/cleaned_data"
export LOG_DATA_DIR="$ROOT_DIR/log_data"


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

if [ ! -d $LOG_DATA_DIR ]; then
  print_info "creating $LOG_DATA_DIR"
  mkdir -p $LOG_DATA_DIR
else
  rm -rf $LOG_DATA_DIR/*
fi

echo ""

./test.py \
  --input=$FILE_NAMES \
  --output=$CLEAN_DATA_DIR \
  --log=$LOG_DATA_DIR \
  --events=$NUM_EVENTS \
  --actions=$NUM_ACTIONS \
  --threads=14
