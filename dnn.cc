#include "perceptron.h"

#include <random>
#include <algorithm>
#include <cmath>

DNN::DNN(size_t actions, size_t features, int init_range, double eta) {
  this->actions.resize(actions, Perceptron(features, init_range, eta));
}

int
DNN::eval(uint64_t pc, std::vector<uint8_t> signature) {
  std::vector<double> confidence;

  for(size_t i = 0; i < actions.size(); i++) {
    confidence.push_back(actions[i].eval(pc, signature));
  }

  auto result = std::max_element(confidence.begin(), confidence.end());
  return std::distance(confidence.begin(), result);
}

void
DNN::train(uint64_t pc, std::vector<uint8_t> signature, int action) {
  int predicted = eval(pc, signature);
  if(predicted != action) {
    actions[predicted].train(pc, signature, false);
  }
  actions[action].train(pc, signature, true);
}

