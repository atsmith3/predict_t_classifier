#include "options.h"

#include <boost/program_options.hpp>
#include <iostream>
#include <string>

bool Options::parse(int argc, char **argv) {
  // clang-format off
  po::options_description desc("General Options");
  desc.add_options()
    ("help,h", "Display help message")
  ;

  po::options_description io("IO Options");
  io.add_options()
    ("input_csv_train,tr", po::value<std::string>(&input_csv_train), "Input training CSV file")
    ("input_csv_test,ts", po::value<std::string>(&input_csv_test), "Input testing CSV file")
    ("output_folder,o", po::value<std::string>(&output_folder), "Output folder to store weights")
    ("log_folder,l", po::value<std::string>(&log_folder), "Log folder to store accuracy ")
    ("ntrain", po::value<bool>(&ntrain), "If true, skip training phase")
    ("perceptron", po::value<bool>(&perceptron), "If true, use perceptron, if false use DNN")
    ("events,e", po::value<int>(&events), "Integer number of events")
    ("epochs", po::value<int>(&epochs), "Integer number of epochs")
    ("actions", po::value<int>(&actions), "Integer number of actions")
    ("preprocess", po::value<std::string>(&preprocess), "Preprocess Type: RAW, STANDARDIZE, NORMALIZE")
    ("eta", po::value<double>(&eta), "Training Rate")
    ("minibatch_size", po::value<int>(&minibatch_size), "DNN Batch Size")
    ("hidden_layers", po::value<int>(&hidden_layers), "DNN Num Hidden Layers")
    ("hidden_layer_dimension", po::value<int>(&hidden_layer_dimension), "DNN Hidden Layer Size")
  ;

  po::options_description serial("Serialization Options");
  serial.add_options()
    ("serial_fname", po::value<std::string>(&serial_fname), "Write Perceptron or DNN to file")
    ("serial_create", po::value<bool>(&serial_create), "Serial Create")
    ("serial_restore", po::value<bool>(&serial_restore), "Serial Resotre")
  ;

  // clang-format on

  po::options_description all_options;
  all_options.add(io);
  all_options.add(desc);
  all_options.add(serial);

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, all_options), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << all_options << "\n";
    return false;
  }
  if (input_csv_train == "") {
    std::cerr << "Must specify an Input training CSV File; "
                 "\"./rplace_with_executable_name --help\" for more "
                 "options\n";
    return false;
  }
  if (input_csv_test == "") {
    std::cerr << "Must specify an Input test CSV File; "
                 "\"./rplace_with_executable_name --help\" for more "
                 "options\n";
    return false;
  }
  // if (output_folder == "") {
  //  std::cerr << "Must specify an Output Folder; "
  //               "\"./rplace_with_executable_name --help\" for more "
  //               "options\n";
  //  return false;
  //}
  // if (log_folder == "") {
  //  std::cerr << "Must specify an Log Folder; \"./rplace_with_executable_name
  //  "
  //               "--help\" for more "
  //               "options\n";
  //  return false;
  //}
  return true;
}
