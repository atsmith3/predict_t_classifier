import pandas as pd
import numpy as np
import argparse
import os
import glob
import math
import threading, queue
from sklearn.model_selection import train_test_split

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
    help="Number of actions to take")
parser.add_argument(
    '--no-pc',
    action='store_true',
    help="Do not include the PCs in the output data")
parser.add_argument(
    '--anchor-pc',
    action='store_true',
    help="Only include the Anchor PC in the output data")
parser.add_argument(
    '--num-events',
    type=int,
    default=256,
    help="Number of events")
parser.add_argument(
    '--remove-duplicates-soft',
    action='store_true',
    help="Remove Duplicate entries from the same set of 10 actions")
parser.add_argument(
    '--remove-duplicates-hard',
    action='store_true',
    help="Remove Duplicate entries from entire set (Perceptron Classifier?)")
parser.add_argument(
    '--distribute-current',
    action='store_true',
    help="Distribute the current across the set of 10 prints")
parser.add_argument(
    '--shunt',
    action='store_true',
    help="Does the system have a shunt system to handle load release?")
args = parser.parse_args()

sys_voltage = []                # Column 0; Voltage from VAMS
sys_current = []                # Column 1; Current from VAMS
core_runtime_curr = []          # Column 2; Core Runtime Current from McPAT
core_runtime_curr_di = []       # Column 3: Core Runtime Current di from McPAT
total_runtime_curr = []         # Column 4; Total Runtime Current from McPAT
total_runtime_curr_di = []      # Column 5: Total Runtime Current di from McPAT
#history_dump_pc = [[]]*256      # Column 6: History Dump PC Values
#history_dump_event = [[]]*256   # Column 6: History Dump Event Values
history_dump_pc = []      # Column 6: History Dump PC Values
history_dump_event = []   # Column 6: History Dump Event Values
policy = []

maximal_core_di = 0.0           # Used for generating the range of Throttle Actions
minimal_core_di = 0.0           # Used for generating the range of Shunt Actions

## get_files
#
#  Python Glob to get files from the input path
#  @param path Input Path
#  @return sorted list of files
def get_files(path):
  files = glob.glob(path+"/*.csv")
  files = [i for i in files]
  files.sort()
  return files

## read_data
#
#  Reads in the input files, tokenizes and adds tokens to the global lists
#  @param file array to read
#  @return num_lines_read
def read_data(files):
  global sys_voltage
  global sys_current
  global core_runtime_curr
  global core_runtime_curr_di
  global total_runtime_curr
  global total_runtime_curr_di
  global history_dump_pc
  global history_dump_event
  global maximal_core_di
  global minimal_core_di

  for f in files:
    with open(f, "r") as infile:
      for line in infile.readlines():
        pc = []
        event = []
        if("#" == line.strip()[0]):
          # Skip line, '#' is a comment
          continue
        for token,i in zip(line.strip().split(","),range(len(line.strip().split(",")))):
          if i == 0:
            sys_voltage.append(float(token))
          if i == 1:
            sys_current.append(float(token))
          if i == 2:
            core_runtime_curr.append(float(token))
          if i == 3:
            core_runtime_curr_di.append(float(token))
            if float(token) > maximal_core_di:
              maximal_core_di = float(token)
            if float(token) < minimal_core_di:
              minimal_core_di = float(token)
          if i == 4:
            total_runtime_curr.append(float(token))
          if i == 5:
            total_runtime_curr_di.append(float(token))
          if (i - 6) in range(256):
            pc.append(int(token,16))
          if (i - (6+256)) in range(256):
            event.append(int(token))
        history_dump_pc.append(pc)
        history_dump_event.append(event)
  print([minimal_core_di,maximal_core_di])
  return len(sys_voltage)

## read_data
#
#  Reads in the input files, tokenizes and adds tokens to the global lists
#  @param file array to read
#  @return num_lines_read
def write_data(file):
  global sys_voltage
  global sys_current
  global core_runtime_curr
  global core_runtime_curr_di
  global total_runtime_curr
  global total_runtime_curr_di
  global history_dump_pc
  global history_dump_event
  global maximal_core_di
  global minimal_core_di

  with open(file, "w") as infile:
    for j in range(len(sys_voltage)):
      for i in range(8):
        if i == 0:
          infile.write(str(sys_voltage[j])+",")
        if i == 1:
          infile.write(str(sys_current[j])+",")
        if i == 2:
          infile.write(str(core_runtime_curr[j])+",")
        if i == 3:
          infile.write(str(core_runtime_curr_di[j])+",")
        if i == 4:
          infile.write(str(total_runtime_curr[i])+",")
        if i == 5:
          infile.write(str(total_runtime_curr_di[i])+",")
        if (i == 6):
          infile.write(str(",".join([str(i) for i in history_dump_pc[j]])+","))
        if (i == 7):
          infile.write(str(",".join([str(i) for i in history_dump_event[j]])))
      infile.write("\n")
  return len(sys_voltage)

## distribute_current
#
#  Distributes the dI across all the entries in a block of 10 cycles
#  @return len_arrays
def distribute_current():
  global sys_voltage
  global sys_current
  global core_runtime_curr
  global core_runtime_curr_di
  global total_runtime_curr
  global total_runtime_curr_di
  global history_dump_pc
  global history_dump_event

  temp_sys_voltage = []                # Voltage from VAMS
  temp_sys_current = []                # Current from VAMS
  temp_core_runtime_curr = []          # Core Runtime Current from McPAT
  temp_core_runtime_curr_di = []       # Core Runtime Current di from McPAT
  temp_total_runtime_curr = []         # Total Runtime Current from McPAT
  temp_total_runtime_curr_di = []      # Total Runtime Current di from McPAT
  temp_history_dump_pc = []
  temp_history_dump_event = []

  # Start with
  block_count = 0
  cmpstr = "" # Starts over every block; or when there is a diff in cmpstr with
              # the current ehr
  di = 0.0
  for i in range(len(sys_voltage)):
    if(block_count == 9):
      block_count = 0
    if(block_count == 0):
      di = core_runtime_curr_di[i]
    # Push into new arrays
    temp_sys_voltage.append(sys_voltage[i])
    temp_sys_current.append(sys_current[i])
    temp_core_runtime_curr.append(core_runtime_curr[i])
    temp_core_runtime_curr_di.append(di)
    temp_total_runtime_curr.append(total_runtime_curr[i])
    temp_total_runtime_curr_di.append(total_runtime_curr_di[i])
    temp_history_dump_pc.append(history_dump_pc[i])
    temp_history_dump_event.append(history_dump_event[i])
    block_count += 1
  sys_voltage = temp_sys_voltage
  sys_current = temp_sys_current
  rore_runtime_curr = temp_core_runtime_curr
  core_runtime_curr_di = temp_core_runtime_curr_di
  total_runtime_curr = temp_total_runtime_curr
  total_runtime_curr_di = temp_total_runtime_curr_di
  history_dump_pc = temp_history_dump_pc
  history_dump_event = temp_history_dump_event
  return len(sys_voltage)

## trim
#
#  Trim the dimensions of the EHR to specified size
#  @param events Number of events to use
#  @param no_pc Use dont use PC
#  @param anchor_pc Only use Anchor PC
#  @return len_arrays
def trim(events, no_pc=False, anchor_pc=False):
  global sys_voltage
  global sys_current
  global core_runtime_curr
  global core_runtime_curr_di
  global total_runtime_curr
  global total_runtime_curr_di
  global history_dump_pc
  global history_dump_event

  temp_sys_voltage = []                # Voltage from VAMS
  temp_sys_current = []                # Current from VAMS
  temp_core_runtime_curr = []          # Core Runtime Current from McPAT
  temp_core_runtime_curr_di = []       # Core Runtime Current di from McPAT
  temp_total_runtime_curr = []         # Total Runtime Current from McPAT
  temp_total_runtime_curr_di = []      # Total Runtime Current di from McPAT
  temp_history_dump_pc = []
  temp_history_dump_event = []

  # Start with
  for i in range(len(sys_voltage)):
    # Push into new arrays
    temp_sys_voltage.append(sys_voltage[i])
    temp_sys_current.append(sys_current[i])
    temp_core_runtime_curr.append(core_runtime_curr[i])
    temp_core_runtime_curr_di.append(core_runtime_curr_di[i])
    temp_total_runtime_curr.append(total_runtime_curr[i])
    temp_total_runtime_curr_di.append(total_runtime_curr_di[i])
    if(not no_pc and not anchor_pc):
      temp_history_dump_pc.append([history_dump_pc[i][j] for j in range(events)])
    elif(not no_pc and anchor_pc):
      temp_history_dump_pc.append([history_dump_pc[i][j] for j in range(1)])
    elif(no_pc):
      temp_history_dump_pc.append([history_dump_pc[i][j] for j in range(0)])
    temp_history_dump_event.append([history_dump_event[i][j] for j in range(events)])
  sys_voltage = temp_sys_voltage
  sys_current = temp_sys_current
  rore_runtime_curr = temp_core_runtime_curr
  core_runtime_curr_di = temp_core_runtime_curr_di
  total_runtime_curr = temp_total_runtime_curr
  total_runtime_curr_di = temp_total_runtime_curr_di
  history_dump_pc = temp_history_dump_pc
  history_dump_event = temp_history_dump_event
  return len(sys_voltage)


## remove_duplicates_soft()
#
#  Rempve duplicate entries in a series, deduped values use min or max current
#  they are associated with
#  @return num_lines
def remove_duplicates_soft():
  global sys_voltage
  global sys_current
  global core_runtime_curr
  global core_runtime_curr_di
  global total_runtime_curr
  global total_runtime_curr_di
  global history_dump_pc
  global history_dump_event

  temp_sys_voltage = []                # Voltage from VAMS
  temp_sys_current = []                # Current from VAMS
  temp_core_runtime_curr = []          # Core Runtime Current from McPAT
  temp_core_runtime_curr_di = []       # Core Runtime Current di from McPAT
  temp_total_runtime_curr = []         # Total Runtime Current from McPAT
  temp_total_runtime_curr_di = []      # Total Runtime Current di from McPAT
  temp_history_dump_pc = []
  temp_history_dump_event = []

  # Start with
  cmpstr = "" # Every time the EHR Updates
  min_sys_voltage = 0.0
  max_sys_voltage = 0.0
  max_di_dt = 0.0
  min_di_dt = 0.0
  for i in range(len(sys_voltage)):
    cmpstr_prev = cmpstr
    cmpstr = ",".join([str(history_dump_pc[i][k]) for k in range(len(history_dump_pc[i]))] + \
                      [str(history_dump_event[i][k]) for k in range(len(history_dump_event[i]))])
    # Check if the di/dt value is a max or a min
    if(sys_voltage[i] > max_sys_voltage):
      max_sys_voltage = sys_voltage[i]
    if(sys_voltage[i] < min_sys_voltage):
      min_sys_voltage = sys_voltage[i]
    if(core_runtime_curr_di[i] > max_di_dt):
      max_di_dt = core_runtime_curr_di[i]
    if(core_runtime_curr_di[i] < min_di_dt):
      min_di_dt = core_runtime_curr_di[i]
    if(cmpstr != cmpstr_prev):
      #print(cmpstr)
      # Push into new arrays
      if(abs(max_sys_voltage) > abs(min_sys_voltage)):
        temp_sys_voltage.append(max_sys_voltage)
      else:
        temp_sys_voltage.append(min_sys_voltage)
      temp_sys_current.append(sys_current[i])
      temp_core_runtime_curr.append([i])
      if(abs(max_di_dt) > abs(min_di_dt)):
        temp_core_runtime_curr_di.append(max_di_dt)
      else:
        temp_core_runtime_curr_di.append(min_di_dt)
      temp_total_runtime_curr.append(total_runtime_curr[i])
      temp_total_runtime_curr_di.append(total_runtime_curr_di[i])
      temp_history_dump_pc.append(history_dump_pc[i])
      temp_history_dump_event.append(history_dump_event[i])
      max_di_dt = 0.0
      min_di_dt = 0.0
      max_sys_voltage = 0.0
      min_sys_voltage = 0.0
  sys_voltage = temp_sys_voltage
  sys_current = temp_sys_current
  rore_runtime_curr = temp_core_runtime_curr
  core_runtime_curr_di = temp_core_runtime_curr_di
  total_runtime_curr = temp_total_runtime_curr
  total_runtime_curr_di = temp_total_runtime_curr_di
  history_dump_pc = temp_history_dump_pc
  history_dump_event = temp_history_dump_event
  print(len(temp_history_dump_pc))
  print(len(temp_history_dump_event))
  return len(sys_voltage)

## remove_duplicates_hard()
#
#  Rempve duplicate entries over the total dataset
#  @return num_lines
def remove_duplicates_hard():
  global sys_voltage
  global sys_current
  global core_runtime_curr
  global core_runtime_curr_di
  global total_runtime_curr
  global total_runtime_curr_di
  global history_dump_pc
  global history_dump_event

  temp_sys_voltage = []                # Voltage from VAMS
  temp_sys_current = []                # Current from VAMS
  temp_core_runtime_curr = []          # Core Runtime Current from McPAT
  temp_core_runtime_curr_di = []       # Core Runtime Current di from McPAT
  temp_total_runtime_curr = []         # Total Runtime Current from McPAT
  temp_total_runtime_curr_di = []      # Total Runtime Current di from McPAT
  temp_history_dump_pc = []
  temp_history_dump_event = []

  # Start with
  cmpstr = "" # Every time the EHR Updates
  entries = {}
  min_sys_voltage = 0.0
  max_sys_voltage = 0.0
  max_di_dt = 0.0
  min_di_dt = 0.0
  # Use dict as a hash to remove all duplicates TODO
  for i in range(len(sys_voltage)):
    cmpstr_prev = cmpstr
    cmpstr = ",".join([str(history_dump_pc[i][k]) for k in range(len(history_dump_pc[i]))] + \
                      [str(history_dump_event[i][k]) for k in range(len(history_dump_event[i]))])
  sys_voltage = temp_sys_voltage
  sys_current = temp_sys_current
  rore_runtime_curr = temp_core_runtime_curr
  core_runtime_curr_di = temp_core_runtime_curr_di
  total_runtime_curr = temp_total_runtime_curr
  total_runtime_curr_di = temp_total_runtime_curr_di
  history_dump_pc = temp_history_dump_pc
  history_dump_event = temp_history_dump_event
  print(len(temp_history_dump_pc))
  print(len(temp_history_dump_event))
  return len(sys_voltage)

## write_file
#
#  Write the training data
#  @param output_file
#  @return num_lines
def write_file(output_file):
  global sys_voltage
  global sys_current
  global core_runtime_curr
  global core_runtime_curr_di
  global total_runtime_curr
  global total_runtime_curr_di
  global history_dump_pc
  global history_dump_event

  with open(output_file, "w") as ofile:
    for i in range(len(core_runtime_curr_di)):
      ofile.write(",".join( [str(policy[i])] + \
          [str(history_dump_pc[i][k]) for k in range(len(history_dump_pc[i]))] + \
          [str(history_dump_event[i][k]) for k in range(len(history_dump_event[i]))])+"\n")

def assign_expert_policy(actions, maximal, minimal, shunt=False):
  def remap(n, r0, r1):
    d0 = r0[1]-r0[0]
    d1 = r1[1]-r1[0]
    return int(math.floor(d1*(n-r0[0])/d0) + r1[0])

  global sys_voltage
  global sys_current
  global core_runtime_curr
  global core_runtime_curr_di
  global total_runtime_curr
  global total_runtime_curr_di
  global history_dump_pc
  global history_dump_event

  policy = []

  for i in range(len(history_dump_event)):
    if(shunt):
      # TODO:
      if(core_runimte_curr[i] < 0 and shunt):
        policy.append(0)
    elif(core_runtime_curr_di[i] > 0):
      if(core_runtime_curr_di[i]/maximal > 0.5):
        policy.append(actions-1)
      else:
        policy.append(remap(core_runtime_curr_di[i]/maximal, [0,0.5], [0,actions-1]))
    else:
      policy.append(0)
  return policy

files = get_files(args.input)
print(files)
lines = read_data(files)
print("After read_data: Lines= "+str(lines))
lines = trim(args.num_events, args.no_pc, args.anchor_pc)
print("After trim: Lines= "+str(lines))
lines = distribute_current()
print("After distribute_current: Lines= "+str(lines))
lines = remove_duplicates_soft()
print("After remove_duplicates_soft: Lines= "+str(lines))
policy = assign_expert_policy(args.actions, maximal_core_di, minimal_core_di)
write_data("output.txt")
write_file("train.txt")
