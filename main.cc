#include "perceptron.h"
#include "options.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <string>
#include <fstream>
#include <vector>
#include <cstdint>

#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream



void train(Classifier &a, std::string file_, int num_events_){
  std::ifstream myFile(file_);
  if(!myFile.is_open()) throw std::runtime_error("Could not open file");

  int num_events = num_events_;
  std::vector<uint64_t> pc;
  std::vector<std::vector<uint8_t>> signatures;
  std::vector<int> action;
  std::string file_line,col_value;
  while(myFile.good()){
    getline(myFile,file_line);
    std::stringstream ss(file_line);

    if (!(std::getline(ss, col_value, ','))){ //checking if its an empty line
      continue;
    }
    action.push_back(std::stoi(col_value));

    std::getline(ss, col_value, ',');
    pc.push_back(std::stoi(col_value,0,16));

    std::vector<uint8_t> current_signature;
    int count = 0;
    while(std::getline(ss, col_value, ',')){
      int event_int = std::stoi(col_value);
      uint8_t event_uint8 = event_int;
      current_signature.push_back(event_uint8);
      //cout<<std::stoi(col_value)<<endl;
      count++;
      if(count>num_events){
        throw std::runtime_error("Number of Events specified is less than number of events that exist");
      }
    }
    if(count!=num_events){
        throw std::runtime_error("Number of Events specified is less than the number that exist");
    }
    signatures.push_back(current_signature);
  }
  for(int i=0;i<pc.size();i++){
    a.train(pc[i],signatures[i],action[i]);
  }
  return;
}

std::pair<int,int> test(Classifier &a, std::string file_, int num_events_){
  std::ifstream myFile(file_);
  if(!myFile.is_open()) throw std::runtime_error("Could not open file");

  int num_events = num_events_;
  std::vector<uint64_t> pc;
  std::vector<std::vector<uint8_t>> signatures;
  std::vector<int> action;
  std::string file_line,col_value;
  while(myFile.good()){
    getline(myFile,file_line);
    std::stringstream ss(file_line);

    if (!(std::getline(ss, col_value, ','))){ //checking if its an empty line
      continue;
    }
    action.push_back(std::stoi(col_value));

    std::getline(ss, col_value, ',');
    pc.push_back(std::stoi(col_value,0,16));

    std::vector<uint8_t> current_signature;
    int count = 0;
    while(std::getline(ss, col_value, ',')){
      int event_int = std::stoi(col_value);
      uint8_t event_uint8 = event_int;
      current_signature.push_back(event_uint8);
      //cout<<std::stoi(col_value)<<endl;
      count++;
      if(count>num_events){
        throw std::runtime_error("Number of Events specified is less than number of events that exist");
      }
    }
    if(count!=num_events){
        throw std::runtime_error("Number of Events specified is less than the number that exist");
    }
    signatures.push_back(current_signature);
  }
  int correct_prediction = 0;
  int total_test = pc.size(); 
  for(int i=0;i<pc.size();i++){
    if (action[i] == a.eval(pc[i],signatures[i])) correct_prediction++;
  }

  return std::pair<int,int> (correct_prediction,total_test);
}
int main(int argc, char** argv) {

  Options opt;
  if (!opt.parse(argc, argv)) {
    return 1;
  }

  /** Classifier(actions, signature_len, init_val, eta */
  Classifier a = Classifier(2, opt.number_events, 10, 10.0);

  /** Train */
  train(a,opt.input_csv_train,opt.number_events);

  /** Test */
  std::pair<int,int> test_data = test(a,opt.input_csv_test,opt.number_events) 

  /** Serialize */
  // TODO: Write Custom Serialization Utility

  return 1;
}
