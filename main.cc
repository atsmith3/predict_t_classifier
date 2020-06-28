#include "perceptron.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>

//int main(int argc, char** argv) {
int main() {
  /** Classifier(actions, signature_len, init_val, eta */
  Classifier a = Classifier(2, 256, 10, 10.0);

  /** Train */
  // TODO: Write Train Utility

  /** Test */
  // TODO: Write Test Utility

  /** Serialize */
  // TODO: Write Custom Serialization Utility

  return 1;
}
