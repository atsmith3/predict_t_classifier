#!/usr/bin/env python3
import pandas as pd
import numpy as np
import argparse
import os
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
    '--events', type=int, default=6, help="Number of events used")
parser.add_argument(
    '--actions',
    type=int,
    default=2,
    help="Number of actions to take, MINIMUM 2, MAXIMUM 10")
parser.add_argument('--threads', type=int, default=2, help="Number of threads")
parser.add_argument('--pc', type=bool, default=False, help="Include PC")

args = parser.parse_args()
input_paths = args.input
output_path = args.output
num_events = args.events
num_actions = args.actions
num_threads = 1


def parse_data(InputQueue, OutputQueue_training, OutputQueue_test):

  while not InputQueue.empty():
    file = InputQueue.get()
    if not len(file) > 0:
      continue

    if not (os.path.getsize(file) >= num_events + 3):
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

    print(args.pc)
    if (not args.pc):
      print("Pc Not Specified")
      data.drop([0, 1, 2], axis=1,
                inplace=True)  #Dropping the voltage and current values

    else:
      data.drop([0, 1], axis=1,
                inplace=True)  #Dropping the voltage and current values
      data.drop(
          range(3 + num_events, num_columns), axis=1,
          inplace=True)  #Dropping all the excess event values

    train, test = train_test_split(data, test_size=0.1)

    OutputQueue_test.put(test)
    OutputQueue_training.put(train)

    print("Done cleaning - ", file.split("/")[-1])


def gen_action_list():
  action_list = []
  if (num_actions >= 2 and num_actions <= 10):
    for i in range(num_actions - 1, -1, -1):
      action_list.append(i)
  else:
    raise Exception(
        'Number of actions should be between 2 and 10. The value: {}'.format(
            num_actions))
  return action_list


def write_to_CSV(OutputQueue, test_data, training_data):
  if not (training_data ^ test_data):
    raise Exception('Choose only one of the two')
  if (test_data):
    while not OutputQueue.empty():
      data = OutputQueue.get()
      data.to_csv(
          output_path + "/test_data.csv", mode="a", index=False, header=None)
  if (training_data):
    while not OutputQueue.empty():
      data = OutputQueue.get()
      data.to_csv(
          output_path + "/train_data.csv", mode="a", index=False, header=None)
  return


if __name__ == "__main__":
  v_normal = 0.98
  v_emergency = 0.96

  v_range = v_normal - v_emergency
  v_step = v_range / (float(num_actions - 1))

  action_list = gen_action_list()

  InputQueue = queue.Queue()
  OutputQueue_training = queue.Queue()
  OutputQueue_test = queue.Queue()
  threads = []
  for file in input_paths.split(","):
    InputQueue.put(file)
  for i in range(num_threads):
    thr = threading.Thread(
        target=parse_data,
        args=[InputQueue, OutputQueue_training, OutputQueue_test])
    thr.start()
    threads.append(thr)
  # Join Threads:
  for thr in threads:
    thr.join()

  print("Writing to CSV")

  thr_training = threading.Thread(
      target=write_to_CSV, args=[OutputQueue_training, False, True])
  thr_test = threading.Thread(
      target=write_to_CSV, args=[OutputQueue_test, True, False])

  thr_test.start()
  thr_training.start()

  thr_test.join()
  thr_training.join()
