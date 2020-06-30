#include "utility.h"
#include "array.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <vector>
#include <cmath>
#include <random>
#include <iostream>
#include <fstream>
#include <algorithm>

/**
 * train_dnn
 * Minibatch Train the DNN
 * @param dnn The network to train
 * @param train_data The path to the training dataset
 * @param epochs The number of training intervals
 * @param minibatch_size The size of the minibatch, 1 = sgd
 */
void train_dnn(DNN& dnn,
               std::string train_data,
               int epochs,
               int minibatch_size) {
  std::cout << dnn << " " << train_data << " " << epochs << " " << minibatch_size << std::endl;
}

/**
 * train_dnn
 * Minibatch Train the DNN
 * @param dnn The network to test
 * @param test_data The path to the test dataset
 */
void test_dnn(DNN& dnn,
              std::string test_data) {
  std::cout << dnn << " " << test_data << std::endl;
}
