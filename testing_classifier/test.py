#!/usr/bin/env python3
import pandas as pd
import numpy as np
import argparse
import os
import sys
import datetime
import subprocess
import threading, queue
from sklearn.model_selection import train_test_split

parser = argparse.ArgumentParser()
parser.add_argument(
    '--input',
    type=str,
    default="raw_data/basicmath_100000_10_4_HARVARD_PerceptronPredictor.csv",
    help="Path to folder with all the CSV files")
parser.add_argument(
    '--output',
    type=str,
    default="cleaned_data/",
    help="Path to folder for files to be outputted to")
parser.add_argument(
    '--log',
    type=str,
    default="log_data/",
    help="Path to folder for log files")
parser.add_argument(
    '--events', type=int, default=6, help="Number of events, starting at 3")
parser.add_argument(
    '--actions',
    type=int,
    default=2,
    help="Number of actions starting at 2")
parser.add_argument('--threads', type=int, default=2, help="Number of threads")

args = parser.parse_args()
input_paths = args.input
output_path = args.output
num_events_ = args.events
num_actions_ = args.actions
num_threads = args.threads
log_path = args.log


    
def parse_data(InputQueue, OutputList_training, OutputList_test,action_list,num_actions):
  v_normal = 0.98
  v_emergency = 0.96

  v_range = v_normal - v_emergency
  v_step = v_range / (float(num_actions - 1))
  while not InputQueue.empty():
    file = InputQueue.get()
    if not len(file) > 0:
      continue

    if not (os.path.getsize(file) >= 3):
      print(
          "This file does not have any data OR this file does not have enough events as required (look at the events argument in the bash script)\n",
          file.split("/")[-1], "\n")
      continue

    data = pd.read_csv(file, header=None)
    num_columns = len(data.columns)
    coloumn_names = list(range(0, num_columns))

    throttle_data = []
    for index, row in data.iterrows():
      if (row[0] - v_emergency) < 0:
        throttle_data.append(action_list[0])
      elif (row[0] >= v_normal):
        throttle_data.append(action_list[-1])
      else:
        try:
          throttle_data.append(action_list[int(
              (row[0] + v_step - v_emergency) / v_step)])
        except:
          print(file)
          print(int((row[0] + v_step - v_emergency) / v_step))
          print(index)
    data['throttle'] = throttle_data
    coloumn_names.insert(0, 'throttle')
    data = data[coloumn_names]

    data.drop([0, 1], axis=1,
              inplace=True)  #Dropping the voltage and current values

    train, test = train_test_split(data, test_size=0.1)

    OutputList_test.append(test)
    OutputList_training.append(train)

    print("Done cleaning - ", file.split("/")[-1])


def gen_action_list(num_actions):
  action_list = []
  if (num_actions >= 2 and num_actions <= 10):
    for i in range(num_actions - 1, -1, -1):
      action_list.append(i)
  else:
    raise Exception(
        'Number of actions should be between 2 and 10. The value: {}'.format(
            num_actions))
  return action_list


def write_to_CSV(OutputList, test_data, training_data,num_events):
  if not (training_data ^ test_data):
    raise Exception('Choose only one of the two')  
  if (test_data):
    print("Writing test_data to CSV for {} events".format(num_events))
    if(os.path.exists(output_path + "/test_data.csv")):
      os.remove(output_path + "/test_data.csv")
    for data in OutputList:
      updated_data= data.drop(range(3 + num_events, len(data.columns)+1), axis=1)  #Dropping all the excess event values
      updated_data.to_csv(
          output_path + "/test_data.csv", mode="a", index=False, header=None)
  if (training_data):
    print("Writing training_data to CSV for {} events".format(num_events))
    if(os.path.exists(output_path + "/train_data.csv")):
      os.remove(output_path + "/train_data.csv")
    for data in OutputList:
      updated_data = data.drop(range(3 + num_events, len(data.columns)+1), axis=1)  #Dropping all the excess event values
      updated_data.to_csv(
          output_path + "/train_data.csv", mode="a", index=False, header=None)
  return

def run_classifier(num_actions,num_events):
  print("Running classifier for Events-", num_events, "Actions-",num_actions)
  infile1 = os.path.join(output_path, "train_data.csv")
  infile2 = os.path.join(output_path, "test_data.csv")

  # stdo = os.path.join(output_path, "test_data.out")
  # stde = os.path.join(output_path, "test_data.err")
  stdo = log_path + "/out.csv"
  stde = log_path + "/out.err"
  with open(stdo, "w") as so, open(stde, "w") as se:
    p = subprocess.Popen([
        "../build/vs_classifier",
        "-s",
        infile1,
        "-r",
        infile2,
        "-e",
        str(num_events),
        "-a",
        str(num_actions),
    ],
                         stdout=so,
                         stderr=se)
    p.wait()
    filesize = os.path.getsize(stde) 
    if filesize != 0:
      raise Exception("Error occured while running classifier,Events-", num_events, "Actions-",num_actions)
    model_data = pd.read_csv(stdo, header=None)
    model_data.to_csv(
          output_path + "/final_results.csv", mode="a", index=False, header=None)
    return


if __name__ == "__main__":
  for a in range(2,num_actions_+1):
    print("\n\n\n\n")
    print("******************************************* {} ACTIONS *****************************************".format(a))
    action_list_ = gen_action_list(a)
    InputQueue = queue.Queue()
    OutputList_training = []
    OutputList_test = []
    threads = []
    for file in input_paths.split(","):
      InputQueue.put(file)
    for i in range(num_threads):
      thr = threading.Thread(
          target=parse_data,
          args=[InputQueue, OutputList_training, OutputList_test,action_list_,a])
      thr.start()
      threads.append(thr)
    # Join Threads:
    for thr in threads:
      thr.join()
    for e in range(4,num_events_+1):
      print("\n\n")
      thr_training = threading.Thread(
          target=write_to_CSV, args=[OutputList_training, False, True,e])
      thr_test = threading.Thread(
          target=write_to_CSV, args=[OutputList_test, True, False,e])

      thr_test.start()
      thr_training.start()

      thr_test.join()
      thr_training.join()

      run_classifier(a,e)

