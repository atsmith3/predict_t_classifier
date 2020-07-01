#include "dnn.h"
#include "options.h"
#include "perceptron.h"
#include "utility.h"

int main(int argc, char **argv) {
  Options opt;
  if (!opt.parse(argc, argv)) {
    return 1;
  }

  //dnn_unit_test();
  //return 1;

  // std::cout << std::setprecision(0);
  // std::cout << std::fixed;

  bool classifier = true;

  /** Classifier(actions, signature_len, init_val, eta */
  Classifier a = Classifier(opt.actions, opt.events, opt.eta, 0.1);
  DNN dnn = DNN(
      opt.events, opt.actions, opt.hidden_layer_dimension, opt.hidden_layers, opt.eta, 10.0);

  Array2D input_data = read_data(opt.input_csv_train, opt.events, RAW);

  /** Train */
  if (classifier) {
    train_classifier(a, input_data, opt.epochs);
  } else {
    train_dnn(dnn, input_data, opt.epochs, opt.minibatch_size);
  }

  input_data = read_data(opt.input_csv_test, opt.events, RAW);

  /** Test */
  if (classifier) {
    test_classifier(a, input_data);
  } else {
    test_dnn(dnn, input_data);
  }

  /** Serialize */
  return 0;
}
