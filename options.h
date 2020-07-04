#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <boost/program_options.hpp>
#include <iostream>
#include <string>

namespace po = boost::program_options;

class Options {
public:
  // IO Options
  std::string input_csv_train = "";
  std::string input_csv_test = "";
  std::string output_folder = "";
  std::string log_folder = "";
  bool perceptron = true;
  int events = 256;
  int epochs = 10000;
  int actions = 2;
  std::string preprocess = "RAW";
  double eta = 0.1;
  int minibatch_size = 128;
  int hidden_layers = 1;
  int hidden_layer_dimension = 32;
  bool serial_create = false;
  bool serial_restore = false;
  bool ntrain = false;
  std::string serial_fname = "classifier.txt";
  // Get Options from Command Lineinpu
  bool parse(int argc, char **argv);
}; // class Options

#endif // __OPTIONS_H__
