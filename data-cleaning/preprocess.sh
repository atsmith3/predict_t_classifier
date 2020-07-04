#!/bin/bash

ROOT="/scratch/predict-T/training_data"
DIRS=("${ROOT}/training_data_2_mobile_raw" "${ROOT}/training_data_2_laptop_raw" "${ROOT}/training_data_2_desk_raw")
CPU=("MOBILE" "LAPTOP" "DESKTOP")

EVENTS=('4' '8' '16' '64')
ACTIONS=('2' '8')
FLAG=("" "--no-pc" "--anchor-pc")
FLAG_=("all" "npc" "apc")

for i in ${!CPU[@]}; do
  for j in ${!EVENTS[@]}; do
    for k in ${!ACTIONS[@]}; do
      for l in ${!FLAG[@]}; do
        TN=${CPU[$i]}_${EVENTS[$j]}_${ACTIONS[$k]}_${FLAG_[$l]}
echo "
python3 ./preprocess.py \
--input=${DIRS[$i]} \
--num-events=${EVENTS[$j]} \
--actions=${ACTIONS[$k]} \
${FLAG[$l]} \
--output=$TN.csv"
        python3 ./preprocess.py \
          --input=${DIRS[$i]} \
          --num-events=${EVENTS[$j]} \
          --actions=${ACTIONS[$k]} \
          ${FLAG[$l]} \
          --output=$TN.csv
      done
    done
  done
done
