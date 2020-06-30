#include "dnn.h"
#include "utility.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <iomanip>



//int main(int argc, char** argv) {
int main() {
  //std::cout << std::setprecision(0);
  //std::cout << std::fixed;

  DNN classifier = DNN(1+4, 2, 8, 1, 0.1, 10.0);

  /** Train */
  train_dnn(classifier, "train.csv", 10000, 128);

  /** Test */
  test_dnn(classifier, "test.csv");

  /** Serialize */
  return 1;
}
