#!/bin/bash

./build/vs_classifier \
  --input_csv_train='./data-cleaning/optdigits-orig_train.csv' \
  --input_csv_test='./data-cleaning/optdigits-orig_test.csv' \
  --events=1024 \
  --epochs=128 \
  --actions=10 \
  --eta=0.25 \
  --minibatch_size=128 \
  --hidden_layers=1 \
  --hidden_layer_dimension=32 \
  --perceptron=false
