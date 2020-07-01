#!/bin/bash

./build/vs_classifier \
  --input_csv_train='./data-cleaning/optdigits-orig_train.csv' \
  --input_csv_test='./data-cleaning/optdigits-orig_test.csv' \
  --events=1024 \
  --epochs=64 \
  --actions=10 \
  --eta=0.25
