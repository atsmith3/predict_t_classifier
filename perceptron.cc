#include "perceptron.h"

#include <algorithm>
#include <cmath>
#include <random>

Perceptron::Perceptron(size_t features, int init_range, double eta) {
  w.resize(features);
  for (size_t i = 0; i < w.size(); i++) {
    w[i] = random() % init_range;
  }
  pc_w = random() % init_range;
  this->eta = eta;
}

double Perceptron::eval(uint64_t pc, std::vector<uint8_t> signature) const {
  double accum = 0.0;
  for (size_t i = 0; i < signature.size(); i++) {
    accum += w[i] * (double)signature[i];
  }
  accum += pc_w * (double)pc;
  return accum;
}

void Perceptron::train(uint64_t pc,
                       std::vector<uint8_t> signature,
                       bool correct) {
  for (size_t i = 0; i < signature.size(); i++) {
    if (correct) {
      w[i] += (double)signature[i] * eta;
    } else {
      w[i] -= (double)signature[i] * eta;
    }
  }
  if (correct) {
    pc_w += (double)pc * eta;
  } else {
    pc_w -= (double)pc * eta;
  }
}

Classifier::Classifier(size_t actions,
                       size_t features,
                       int init_range,
                       double eta) {
  this->actions.resize(actions, Perceptron(features, init_range, eta));
}

int Classifier::eval(uint64_t pc, std::vector<uint8_t> signature) {
  std::vector<double> confidence;

  for (size_t i = 0; i < actions.size(); i++) {
    confidence.push_back(actions[i].eval(pc, signature));
  }

  auto result = std::max_element(confidence.begin(), confidence.end());
  return std::distance(confidence.begin(), result);
}

void Classifier::train(uint64_t pc,
                       std::vector<uint8_t> signature,
                       int action) {
  int predicted = eval(pc, signature);
  if (predicted != action) {
    actions[predicted].train(pc, signature, false);
  }
  actions[action].train(pc, signature, true);
}
