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
  int number_events;
  // Get Options from Command Lineinpu
  bool parse(int argc, char **argv);
}; // class Options

#endif // __OPTIONS_H__