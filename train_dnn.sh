#!/bin/bash

ROOT="$HOME/research/classifier"

TRAINED="$ROOT/trained_models"
OUTPUT="$ROOT/output"

INPUT="$ROOT/data-cleaning"
#CPU=("MOBILE" "LAPTOP" "DESKTOP")
CPU=("MOBILE")

EVENTS=('8' '4' '5' '16' '8' '9' '32' '16' '17' '128' '64' '65')
EVENTS_=('4' '4' '4' '8' '8' '8' '16' '16' '16' '64' '64' '64')
ACTIONS=('2' '8')
FLAG_=("all" "npc" "apc" "all" "npc" "apc" "all" "npc" "apc" "all" "npc" "apc" )

# DNN Configuration
HIDDEN_LAYER_DIM=('8' '16' '32')
HIDDEN_LAYERS=('1' '2' '4')

for i in ${!CPU[@]}; do
  for j in ${!EVENTS[@]}; do
    for k in ${!ACTIONS[@]}; do
      for l in ${!HIDDEN_LAYERS[@]}; do
        for m in ${!HIDDEN_LAYER_DIM[@]}; do
          sleep 1
          FILE="${INPUT}/${CPU[$i]}_${EVENTS_[$j]}_${ACTIONS[$k]}_${FLAG_[$j]}.csv"
          TN=${CPU[$i]}_${EVENTS[$j]}_${ACTIONS[$k]}_${HIDDEN_LAYERS[$l]}_${HIDDEN_LAYER_DIM[$m]}
          if [ -f "$FILE" ]; then 
echo "
./build/vs_classifier \
--input_csv_train=$FILE \
--input_csv_test=$FILE \
--events=${EVENTS[$j]} \
--epochs=10000 \
--actions=${ACTIONS[$k]} \
--eta=0.25 \
--minibatch_size=128 \
--hidden_layers=${HIDDEN_LAYERS[$l]} \
--hidden_layer_dimension=${HIDDEN_LAYER_DIM[$m]} \
--perceptron=false \
--serial_fname="$TRAINED/dnn_$TN.txt" \
--serial_create=true > $OUTPUT/dnn_$TN.out"
            ./build/vs_classifier \
              --input_csv_train=$FILE \
              --input_csv_test=$FILE \
              --events=${EVENTS[$j]} \
              --epochs=10000 \
              --actions=${ACTIONS[$k]} \
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
