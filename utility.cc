#include "utility.h"

#include "array.h"
#include "func.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

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
               int minibatch_size) {
  std::cout << dnn << " " << train_data << " " << epochs << " "
            << minibatch_size << std::endl;
}

/**
 * train_dnn
 * Test the DNN
 * @param dnn The network to test
 * @param test_data The path to the test dataset
 */
void test_dnn(DNN &dnn, std::string test_data) {
  std::cout << dnn << " " << test_data << std::endl;
}

/**
 * dnn_unit_tests
 * Unit test some of the functionality of the Array2D and Layer Classes
 */
void dnn_unit_test() {
#ifdef DEBUG

  /*
   * Test the Default Contructor of Array2D
   */
  std::cout << "-----------------------------------------------------\n";
  std::cout << "Test 1: Create Array2D\n";
  std::cout << "-----------------------------------------------------\n";
  Array2D A = Array2D();
  std::cout << A << "\n";
  std::cout << "\n\n";

  /*
   * Test the NxM constructor & random Init
   */
  std::cout << "-----------------------------------------------------\n";
  std::cout << "Test 2: Create Array2D; Height=4, Width=6\n";
  std::cout << "-----------------------------------------------------\n";
  A = Array2D(4, 6);
  std::cout << A << "\n";
  std::cout << "\n\n";

  /*
   * Test Multiplication of Array and Scalar
   */
  std::cout << "-----------------------------------------------------\n";
  std::cout << "Test 3: Multiply Array2D*double\n";
  std::cout << "-----------------------------------------------------\n";
  A = Array2D(4, 4, 1.0);
  Array2D B = A * 100.0;
  std::cout << A << "\n";
  std::cout << B << "\n";
  std::cout << "\n\n";

  /*
   * Test Multiplication of Array and Array
   */
  std::cout << "-----------------------------------------------------\n";
  std::cout << "Test 4: Multiply Array2D 2x2, 2x2\n";
  std::cout << "-----------------------------------------------------\n";
  A = Array2D(2, 2, 1.0);
  B = Array2D(2, 2, 1.0);
  Array2D C = A * B;
  std::cout << A << "\n";
  std::cout << B << "\n";
  std::cout << C << "\n";
  std::cout << "\n\n";

  /*
   * Test Multiplication of Array and Array
   */
  std::cout << "-----------------------------------------------------\n";
  std::cout << "Test 5: Multiply Array2D 2x5, 5x2\n";
  std::cout << "-----------------------------------------------------\n";
  A = Array2D(2, 5, 1.0);
  B = Array2D(5, 2, 1.0);
  C = A * B;
  std::cout << A << "\n";
  std::cout << B << "\n";
  std::cout << C << "\n";
  std::cout << "\n\n";

  /*
   * Test Addition of Array and Bias Vector
   */
  std::cout << "-----------------------------------------------------\n";
  std::cout << "Test 6: Addition Array2D 2x5, 5x1\n";
  std::cout << "-----------------------------------------------------\n";
  A = Array2D(2, 5, 1.0);
  B = Array2D(5, 1, 1.0);
  C = A + B;
  std::cout << A << "\n";
  std::cout << B << "\n";
  std::cout << C << "\n";
  std::cout << "\n\n";

  /*
   * Test Subtraction of Array and Array
   */
  std::cout << "-----------------------------------------------------\n";
  std::cout << "Test 7: Subtract Array2D 8x8, 8x8\n";
  std::cout << "-----------------------------------------------------\n";
  A = Array2D(8, 8, 1.0);
  B = A;
  C = A - B;
  std::cout << A << "\n";
  std::cout << B << "\n";
  std::cout << C << "\n";
  std::cout << "\n\n";

  /*
   * Test ReLU Function
   */
  std::cout << "-----------------------------------------------------\n";
  std::cout << "Test 8: Array2D Apply relu()\n";
  std::cout << "-----------------------------------------------------\n";
  A = Array2D(4, 4, 10.0);
  B = A.apply(relu);
  std::cout << A << "\n";
  std::cout << B << "\n";
  std::cout << "\n\n";

  /*
   * Test Argmax()
   */
  std::cout << "-----------------------------------------------------\n";
  std::cout << "Test 9: argmax(Array2D)\n";
  std::cout << "-----------------------------------------------------\n";
  A = Array2D(1, 10, 10.0);
  std::cout << A << "\n";
  std::cout << argmax(A) << "\n";
  std::cout << "\n\n";

  /*
   * Test normalize
   */
  std::cout << "-----------------------------------------------------\n";
  std::cout << "Test 10: normalize(Array2D) 1x10\n";
  std::cout << "-----------------------------------------------------\n";
  A = Array2D(1, 10, 10.0);
  std::cout << A << "\n";
  std::cout << normalize(A) << "\n";
  std::cout << "\n\n";

  /*
   * Test normalize
   */
  std::cout << "-----------------------------------------------------\n";
  std::cout << "Test 11: normalize(Array2D) 10x1\n";
  std::cout << "-----------------------------------------------------\n";
  A = Array2D(10, 1, 10.0);
  std::cout << A << "\n";
  std::cout << normalize(A) << "\n";
  std::cout << "\n\n";

  /*
   * Test normalize
   */
  std::cout << "-----------------------------------------------------\n";
  std::cout << "Test 12: normalize(Array2D) 8x8\n";
  std::cout << "-----------------------------------------------------\n";
  A = Array2D(8, 8, 10.0);
  std::cout << A << "\n";
  std::cout << normalize(A) << "\n";
  std::cout << "\n\n";

  /*
   * Test standardize
   */
  std::cout << "-----------------------------------------------------\n";
  std::cout << "Test 13: standardize(Array2D) 1x10\n";
  std::cout << "-----------------------------------------------------\n";
  A = Array2D(1, 10, 10.0);
  std::cout << A << "\n";
  std::cout << standardize(A) << "\n";
  std::cout << "\n\n";

  /*
   * Test standardize
   */
  std::cout << "-----------------------------------------------------\n";
  std::cout << "Test 14: standardize(Array2D) 10x1\n";
  std::cout << "-----------------------------------------------------\n";
  A = Array2D(10, 1, 10.0);
  std::cout << A << "\n";
  std::cout << standardize(A) << "\n";
  std::cout << "\n\n";

  /*
   * Test standardize
   */
  std::cout << "-----------------------------------------------------\n";
  std::cout << "Test 15: standardize(Array2D) 8x8\n";
  std::cout << "-----------------------------------------------------\n";
  A = Array2D(8, 8, 10.0);
  std::cout << A << "\n";
  std::cout << standardize(A) << "\n";
  std::cout << "\n\n";
#endif
}
