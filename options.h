#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <boost/program_options.hpp>
#include <iostream>
#include <string>


namespace po = boost::program_options;

class Options {
public:
  // IO Options
  std::string input_xml = "";
  int print_level = 2;

  // Optimization Options
  bool opt_for_clk = true;

  // Serialization Options
  std::string serialization_file = "";
  bool serialization_create = false;
  bool serialization_restore = false;

  // Get Options from Command Line
  bool parse(int argc, char **argv);
}; // class Options

} // namespace mcpat

#endif // __OPTIONS_H__
