#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "dnn.h"
#include "perceptron.h"

#include <string>

/**
 * train_dnn
 * Minibatch Train the DNN
 * @param dnn The network to train
 * @param train_data The path to the training dataset
 * @param epochs The number of training intervals
 * @param minibatch_size The size of the minibatch, 1 = sgd
 */
void train_dnn(DNN &dnn,
               std::string train_data,
               int epochs,
               int minibatch_size);

/**
 * train_dnn
 * Minibatch Train the DNN
 * @param dnn The network to test
 * @param test_data The path to the test dataset
 */
void test_dnn(DNN &dnn, std::string train_data);

#endif // __UTILITY_H__
