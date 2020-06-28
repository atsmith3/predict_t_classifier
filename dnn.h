#ifndef __DNN_H__
#define __DNN_H__

#include <cstdint>
#include <vector>
#include <algorithm>



class DNN3 {
  /* Layer 1 */
  Array2D W1, b1;

  /* Layer 2 */
  Array2D W2, b2;

  /* Layer 3 */
  Array2D W3, b3;

public:
  /** 
   * Constructor
   * Create a signature classifier
   * @param actions The number of actions to take
   * @param features The number of input features
   * @param init_range range for random init
   * @param eta training rate
   */
  DNN(size_t actions,
             size_t features,
             int init_range=10,
             double eta=100.0);
  
  /** 
   * eval
   * Evaluate the classifier
   * @param pc The input anchor PC
   * @param signature The input event signature
   * @return confidence
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

class DNN4 {

public:
  /** 
   * Constructor
   * Create a signature classifier
   * @param actions The number of actions to take
   * @param features The number of input features
   * @param init_range range for random init
   * @param eta training rate
   */
  DNN(size_t actions,
             size_t features,
             int init_range=10,
             double eta=100.0);
  
  /** 
   * eval
   * Evaluate the classifier
   * @param pc The input anchor PC
   * @param signature The input event signature
   * @return confidence
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

#endif // __DNN_H__
