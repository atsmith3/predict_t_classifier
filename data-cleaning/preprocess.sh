#!/bin/bash

ROOT="/scratch/predict-T/training_data"
DIRS=("${ROOT}/training_data_2_mobile_raw" "${ROOT}/training_data_2_laptop_raw" "${ROOT}/training_data_2_desk_raw")
CPU=("MOBILE" "LAPTOP" "DESKTOP")

EVENTS=('64')
FLAG=("")
FLAG_=("all")

for i in ${!CPU[@]}; do
  TN=${CPU[$i]}_norm_norm
echo "
python3 ./preprocess.py \
--input=${DIRS[$i]} \
--output=$TN.csv"
  python3 ./preprocess.py \
    --input=${DIRS[$i]} \
    --output=$TN.csv
done
