# Script to convert flattened 2D Data to 1D vector:
#
# Data has form of:
# Label
# 32x32 array of features with a ' ' as the delimiter
#
# Convert to 1D vector of:
# Label,F0,F1,F2,....
# with "," as the delimiter
#
import pandas as pd
import numpy as np
import argparse
import os
import threading, queue
from sklearn.model_selection import train_test_split

parser = argparse.ArgumentParser()
parser.add_argument(
    '--infile',
    type=str,
    default="",
    help="Input file to convert")
parser.add_argument(
    '--outfile',
    type=str,
    default="",
    help="Output file to write to")
parser.add_argument('--feature_dim', type=int, default=32, help="Feature Dimension")
args = parser.parse_args()

feature_y_dim=args.feature_dim

converted_data = []
with open(args.infile, "r") as infile:
  vect = []
  for line in infile.readlines():
    vect = vect + line.strip().replace("0", "0 ").replace("1", "1 ").strip().split(" ")
    if(len(line.strip()) == 1):
      # New input vector
      # Move the label to the front of the list
      vect = vect[-1:] + vect[:-1]
      converted_data.append(",".join(vect))
      vect = []

with open(args.outfile, "w") as ofile:
  for line in converted_data:
    ofile.write(line+"\n")

