import pandas as pd
import numpy as np
import argparse
import os
import threading, queue
import glob
import math
from sklearn.model_selection import train_test_split
from sklearn import preprocessing
import matplotlib.pyplot as plt
import hashlib
from decimal import *

#getcontext().prec = 100

parser = argparse.ArgumentParser()
parser.add_argument(
    '--input',
    type=str,
    default="",
    help="Path to folder with all the CSV files")
parser.add_argument(
    '--actions',
    type=int,
    default=2,
    help="Actions")
parser.add_argument(
    '--pc',
    type=int,
    default=1,
    help="PCs")
parser.add_argument(
    '--events',
    type=int,
    default=1,
    help="Events")
args = parser.parse_args()

def remap(n, r0, r1):
  d0 = Decimal(r0[1] - r0[0])
  d1 = Decimal(r1[1] - r1[0])
  return Decimal(d1 * (Decimal(n) - r0[0] / d0) + r1[0])

ds_actions = []
ds_pcs = []
ds_events = []

concat_a = []
concat_b = []

# Parse:
with open(args.input, "r") as infile:
  for line in enumerate(infile):
    pc = []
    event = []
    for token, i in zip(line[1].split(","),
                        range(len(line[1].strip().split(",")))):
      if i == 0:
        ds_actions.append(int(token))
      if (args.pc > 0):
        if (i - 1) in range(args.pc):
          pc.append(int(token, 16))
        if (i - (1 + args.pc)) in range(args.events):
          event.append(int(token))
      else:
# Divide so there is something to plot X,Y
        if (i - 1) in range(int(math.floor(args.events/2))):
          pc.append(int(token))
        if (i - (1+int(math.floor(args.events/2)))) in range(int(math.floor(args.events/2))):
          event.append(int(token))
    ds_pcs.append(pc)
    ds_events.append(event)

# Concat:
for i in range(len(ds_actions)):
  concat_a.append(int("".join([str(j) for j in ds_pcs[i]])))
  #print(concat_a[-1])
  concat_b.append(int("".join([str(j) for j in ds_events[i]])))
  #print(concat_b[-1])

# Rescale:
maximum = max(concat_a)
minimum = min(concat_a)
print([minimum, maximum])
for i in range(len(concat_a)):
  concat_a[i] = Decimal(remap(Decimal(concat_a[i]),[Decimal(minimum),Decimal(maximum)],[Decimal(0),Decimal(1)]))
maximum = max(concat_b)
minimum = min(concat_b)
print([minimum, maximum])
for i in range(len(concat_b)):
  concat_b[i] = Decimal(remap(Decimal(concat_b[i]),[Decimal(minimum),Decimal(maximum)],[Decimal(0),Decimal(1)]))


## Standardize:
#s = Decimal(sum(concat_a))
#mean = s/Decimal(len(concat_a))
#inner_sum = 0
#for i in range(len(concat_a)):
#  inner_sum += Decimal((concat_a[i] - mean)*(concat_a[i] - mean))
#stdev = Decimal(math.sqrt(Decimal(inner_sum)))/Decimal(len(concat_a))
#print([mean, stdev])
#for i in range(len(concat_a)):
#  concat_a[i] = (concat_a[i] - mean)/stdev
#
#s = Decimal(sum(concat_b))
#mean = s/Decimal(len(concat_b))
#inner_sum = 0
#for i in range(len(concat_b)):
#  inner_sum += Decimal((concat_b[i] - mean)*(concat_b[i] - mean))
#stdev = Decimal(math.sqrt(Decimal(inner_sum)))/Decimal(len(concat_b))
#print([mean, stdev])
#for i in range(len(concat_b)):
#  concat_b[i] = (concat_b[i] - mean)/stdev

# SKLearn Standardize:
npl = np.array(concat_a)
npl = preprocessing.scale(npl)
concat_a = npl.tolist()

npl = np.array(concat_b)
npl = preprocessing.scale(npl)
concat_b = npl.tolist()

bins_a = [[] for i in range(args.actions)]
bins_b = [[] for i in range(args.actions)]

# Bin:
for i in range(len(concat_a)):
  bins_a[ds_actions[i]].append(concat_a[i])
  bins_b[ds_actions[i]].append(concat_b[i])

#for i in range(len(bins_a)):
#  print(bins_a[i])
#  print(bins_b[i])

labels=[str(i) for i in range(args.actions)]

fig, axs = plt.subplots(1, 1)
fig.set_size_inches(8,6)
for x,y,l in zip(bins_a, bins_b, labels):
  axs.scatter(x, y, label=l, alpha=0.5)
axs.legend()
axs.set_ylabel("A")
axs.set_xlabel("B")
fig.suptitle("Dataset: "+args.input.split(".")[0])
plt.show()
