#ifndef __DNN_H__
#define __DNN_H__

#include "layer.h"

#include <cstdint>
#include <vector>
#include <algorithm>

class DNN {
  Layer input;
  std::vector<Layer> hidden;
  Layer output;

public:
  /** 
   * Constructor
   * Create a 3 Layer DNN, 1 Input, 1 Hidden, 1 Output Layer
   * @param features
   * @param actions
   * @param hidden_dimension
   * @param hidden_layers
   * @param eta
   * @param init
   */
  DNN(size_t features,
       size_t actions,
       size_t hidden_dim,
       size_t hidden_layers,
       double eta = 0.1,
       double init = 1.0);
  
  /** 
   * eval
   * Evaluate the dnn
   * @param input
   * @return Predicted Action
   */
  int eval(Array2D input);

  /** 
   * train
   * Evaluate the classifier and train with Gradient
   * Descent 
   * @param batch
   * @param actions
   * @return loss
   */
  double train(Array2D batch, Array2D actions);

  /**
   * operator<<
   * print out the contents of the DNN for Debugging
   * @param os Output stream to write to
   * @param dnn DNN3 to print
   * @return output stream
   */
  friend std::ostream& operator<<(std::ostream& os, const DNN& dnn) {
    os << dnn.input;
    for(size_t i = 0; i < dnn.hidden.size(); i++) {
      os << dnn.hidden[i];
    }
    os << dnn.output;
    return os;
  }
};

#endif // __DNN_H__
