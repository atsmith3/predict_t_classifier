#include "options.h"

#include <boost/program_options.hpp>
#include <iostream>
#include <string>

bool mcpat::Options::parse(int argc, char **argv) {
  // clang-format off
  po::options_description desc("General Options");
  desc.add_options()
    ("help,h", "Display help message")
  ;

  po::options_description io("IO Options");
  io.add_options()
    ("infile,i", po::value<std::string>(&input_xml), "Input XML File")
    ("print_level,p", po::value<int>(&print_level), "How detailed to print device tree; [1,5] being most detailed");
  ;

  // po::options_description serialization("Serialization Options");
  // serialization.add_options()
  //   ("serial_file", po::value<std::string>(&serialization_file)->default_value("mcpat_cp.txt"), "file name to serialize to")
  //   ("serial_create", po::value<bool>(&serialization_create)->default_value(false), "Create A Serialization Checkpoint") 
  //   ("serial_restore", po::value<bool>(&serialization_restore)->default_value(false), "Restore from a Serialization Checkpoint")
  // ;

  // po::options_description optimization("Optimization Options");
  // optimization.add_options()
  //   ("opt_for_clk,o", po::value<bool>(&opt_for_clk)->default_value(true), "0: optimize for ED^2P only; 1: optimzed for target clock rate")
  // ;
  // clang-format on

  po::options_description all_options;
  all_options.add(desc);
  all_options.add(io);
  // all_options.add(serialization);
  // all_options.add(optimization);

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, all_options), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << all_options << "\n";
    return false;
  }
  if (input_xml == "") {
    std::cerr << "Must specify an Input XML File; \"./mcpat --help\" for more "
                 "options\n";
    return false;
  }
  return true;
}
