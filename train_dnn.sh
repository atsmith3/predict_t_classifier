#!/bin/bash

ROOT="$HOME/research/classifier"

TRAINED="$ROOT/trained_models"
OUTPUT="$ROOT/output"

INPUT="$ROOT/data-cleaning/rescaled_data"
#CPU=("MOBILE" "LAPTOP" "DESKTOP")
CPU=("MOBILE" "LAPTOP" "DESKTOP")

#EVENTS=('128')
#EVENTS_=('64')
#EVENTS=('128' '64' '65')
#EVENTS_=('64' '64' '64')
#EVENTS=('16' '8' '9' '32' '16' '17')
#EVENTS_=('8' '8' '8' '16' '16' '16')
EVENTS=('16' '32')
EVENTS_=('8' '16')
#ACTIONS=('2' '8')
ACTIONS=('2')
#FLAG_=("all" "npc" "apc")
FLAG_=("all" "all")
#FLAG_=("all")

# DNN Configuration
EPOCH=("8192")
HIDDEN_LAYER_DIM=('32')
HIDDEN_LAYERS=('1')
PREPROCESS=('RAW')

for i in ${!CPU[@]}; do
  for j in ${!EVENTS[@]}; do
    for k in ${!ACTIONS[@]}; do
      for l in ${!HIDDEN_LAYERS[@]}; do
        for m in ${!HIDDEN_LAYER_DIM[@]}; do
          for p in ${!PREPROCESS[@]}; do
            sleep 1
            FILE="${INPUT}/${CPU[$i]}_norm_norm_${EVENTS_[$j]}_${ACTIONS[$k]}_${FLAG_[$j]}.csv"
            TN=${CPU[$i]}_${EVENTS[$j]}_${ACTIONS[$k]}_${HIDDEN_LAYERS[$l]}_${HIDDEN_LAYER_DIM[$m]}_${EPOCH[0]}_${PREPROCESS[$p]}
            if [ -f "$FILE" ]; then 
echo "
./build/vs_classifier \
--input_csv_train=$FILE \
--input_csv_test=$FILE \
--events=${EVENTS[$j]} \
--epochs=${EPOCH[0]} \
--actions=${ACTIONS[$k]} \
--preprocess=${PREPROCESS[$p]} \
--eta=0.75 \
--minibatch_size=1024 \
--hidden_layers=${HIDDEN_LAYERS[$l]} \
--hidden_layer_dimension=${HIDDEN_LAYER_DIM[$m]} \
--perceptron=false \
--serial_fname="$TRAINED/dnn_$TN.txt" \
--serial_create=true > $OUTPUT/dnn_$TN.out"
              time ./build/vs_classifier \
                --input_csv_train=$FILE \
                --input_csv_test=$FILE \
                --events=${EVENTS[$j]} \
                --epochs=${EPOCH[0]} \
                --actions=${ACTIONS[$k]} \
                --preprocess=${PREPROCESS[$p]} \
                --eta=0.75 \
                --minibatch_size=1024 \
                --hidden_layers=${HIDDEN_LAYERS[$l]} \
                --hidden_layer_dimension=${HIDDEN_LAYER_DIM[$m]} \
                --perceptron=false \
                --serial_fname="$TRAINED/dnn_${TN}.txt" \
                --serial_create=true > $OUTPUT/dnn_$TN.out &
            fi
            while [ `jobs | wc -l` -ge 8 ]; do
              sleep 1
            done
          done
        done
      done
    done
  done
done
