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
    ("input_csv_train,r", po::value<std::string>(&input_csv_train), "Input training CSV file")
    ("input_csv_test,s", po::value<std::string>(&input_csv_test), "Input testing CSV file")
    // ("output_folder,o", po::value<std::string>(&output_folder), "Output folder to store weights");
    // ("log_folder,l", po::value<std::string>(&log_folder), "Log folder to store accuracy ");
    ("number_events,e", po::value<int>(&number_events), "Integer number of events")
    ("number_actions,a", po::value<int>(&number_actions), "Integer number of actions")

  ;
  // clang-format on

  po::options_description all_options;
  all_options.add(io);
  all_options.add(desc);

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
  //   std::cerr << "Must specify an Output Folder; "
  //                "\"./rplace_with_executable_name --help\" for more "
  //                "options\n";
  //   return false;
  // }
  // if (log_folder == "") {
  //   std::cerr << "Must specify an Log Folder; \"./rplace_with_executable_name "
  //                "--help\" for more "
  //                "options\n";
  //   return false;
  // }
  return true;
}
