#!/bin/bash

#./build/vs_classifier \
#  --input_csv_train='./data-cleaning/cleaned_data_256EVENTS_2ACTIONS/train_data.csv' \
#  --input_csv_test='./data-cleaning/cleaned_data_256EVENTS_2ACTIONS/test_data.csv' \
#  --events=256 \
#  --epochs=64 \
#  --actions=2 \
#  --eta=0.25

./build/vs_classifier \
  --input_csv_train='./data-cleaning/cleaned_data_256EVENTS_2ACTIONS/train_data.csv' \
  --input_csv_test='./data-cleaning/cleaned_data_256EVENTS_2ACTIONS/test_data.csv' \
  --events=256 \
  --epochs=64 \
  --actions=2 \
  --eta=0.25 \
  --minibatch_size=128 \
  --hidden_layers=1 \
  --hidden_layer_dimension=32 \
  --perceptron=false
