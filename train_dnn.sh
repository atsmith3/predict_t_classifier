#!/bin/bash

ROOT="$HOME/research/classifier"

TRAINED="$ROOT/trained_models"
OUTPUT="$ROOT/output"

INPUT="$ROOT/data-cleaning"
#CPU=("MOBILE" "LAPTOP" "DESKTOP")
CPU=("MOBILE" "DESKTOP")

EVENTS=('32' '16' '17' '128' '64' '65')
EVENTS_=('16' '16' '16' '64' '64' '64')
ACTIONS=('2' '8')
FLAG_=("all" "npc" "apc" "all" "npc" "apc")

# DNN Configuration
EPOCH=("10000")
HIDDEN_LAYER_DIM=('32')
HIDDEN_LAYERS=('1' '2')
PREPROCESS=('NORMALIZE' 'STANDARDIZE')

for i in ${!CPU[@]}; do
  for o in ${!EPOCH[@]}; do
    for j in ${!EVENTS[@]}; do
      for k in ${!ACTIONS[@]}; do
        for l in ${!HIDDEN_LAYERS[@]}; do
          for m in ${!HIDDEN_LAYER_DIM[@]}; do
            for p in ${!PREPROCESS[@]}; do
              sleep 1
              FILE="${INPUT}/${CPU[$i]}_${EVENTS_[$j]}_${ACTIONS[$k]}_${FLAG_[$j]}.csv"
              TN=${CPU[$i]}_${EVENTS[$j]}_${ACTIONS[$k]}_${HIDDEN_LAYERS[$l]}_${HIDDEN_LAYER_DIM[$m]}_${EPOCH[$o]}_${PREPROCESS[$p]}
              if [ -f "$FILE" ]; then 
echo "
./build/vs_classifier \
--input_csv_train=$FILE \
--input_csv_test=$FILE \
--events=${EVENTS[$j]} \
--epochs=${EPOCH[$o]} \
--actions=${ACTIONS[$k]} \
--preprocess=${PREPROCESS[$p]} \
--eta=0.25 \
--minibatch_size=128 \
--hidden_layers=${HIDDEN_LAYERS[$l]} \
--hidden_layer_dimension=${HIDDEN_LAYER_DIM[$m]} \
--perceptron=false \
--serial_fname="$TRAINED/dnn_$TN.txt" \
--serial_create=true > $OUTPUT/dnn_$TN.out"
                time ./build/vs_classifier \
                  --input_csv_train=$FILE \
                  --input_csv_test=$FILE \
                  --events=${EVENTS[$j]} \
                  --epochs=${EPOCH[$o]} \
                  --actions=${ACTIONS[$k]} \
                  --preprocess=${PREPROCESS[$p]} \
                  --eta=0.25 \
                  --minibatch_size=128 \
                  --hidden_layers=${HIDDEN_LAYERS[$l]} \
                  --hidden_layer_dimension=${HIDDEN_LAYER_DIM[$m]} \
                  --perceptron=false \
                  --serial_fname="$TRAINED/dnn_$TN.txt" \
                  --serial_create=true > $OUTPUT/dnn_$TN.out &
              fi
              while [ `jobs | wc -l` -ge 32 ]; do
                sleep 1
              done
            done
          done
        done
      done
    done
  done
done
