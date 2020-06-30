#ifndef __PERCEPTRON_H__
#define __PERCEPTRON_H__

#include <algorithm>
#include <cstdint>
#include <vector>

class Perceptron {
  /** weights */
  std::vector<double> w;
  double pc_w;

  /** Eta; training rate */
  double eta;

public:
  /**
   * Constructor
   * Create a signature perceptron
   * @param features The number of input features
   * @param init_range range for random init
   * @param eta training rate
   */
  Perceptron(size_t features, int init_range = 10, double eta = 100.0);

  /**
   * eval
   * Evaluate the perceptron
   * @param pc The input anchor PC
   * @param signature The input event signature
   * @return confidence
   */
  double eval(uint64_t pc, std::vector<uint8_t> signature) const;

  /**
   * train
   * Evaluate the perceptron and train
   * @param pc The input anchor PC
   * @param signature The input event signature
   */
  void train(uint64_t pc, std::vector<uint8_t> signature, bool correct);
};

class Classifier {
  std::vector<Perceptron> actions;

public:
  /**
   * Constructor
   * Create a signature classifier
   * @param actions The number of actions to take
   * @param features The number of input features
   * @param init_range range for random init
   * @param eta training rate
   */
  Classifier(size_t actions,
             size_t features,
             int init_range = 10,
             double eta = 1.5);

  /**
   * eval
   * Evaluate the classifier
   * @param pc The input anchor PC
   * @param signature The input event signature
   * @return action taken
   */
  int eval(uint64_t pc, std::vector<uint8_t> signature);

  /**
   * train
   * Evaluate the classifier and train
   * @param pc The input anchor PC
   * @param signature The input event signature
   * @param action Correct action to take
   */
  void train(uint64_t pc, std::vector<uint8_t> signature, int action);
};

#endif // __PERCEPTRON_H__
