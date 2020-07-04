#!/bin/bash

for pid in $(ps -ef | grep -e "$(whoami).*vs_classifier --input_csv" | awk '{print $2}'); do kill -9 $pid; done
