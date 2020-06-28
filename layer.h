#ifndef __LAYER_H__
#define __LAYER_H__

#include "array.h"

class Layer {
  Array2D W;
  Array2D b;
  Array2D Z;

  // For Reverse
  Array2D dW;
  Array2D db;
  Array2D dZ;
  Array2D dA;

  double eta;

  /**
   * affine_forward
   * compute Z = (A x W) + b
   * @param A Input Array
   * @return Z
   */
  Array2D affine_forward(Array2D A);

  /**
   * relu_forward
   * apply relu to Z
   * @param Z Input array
   * @return ReLU(Z)
   */
  Array2D relu_forward(Array2D Z);

  /**
   * affine_reverse
   * Compute Gradient with respect to input
   * @param dZ output from relu_reverse
   * @return dA
   */
  Array2D affine_reverse(Array2D dZ);

  /**
   * relu_reverse
   * Reverse activation function
   * @param dA output from previous layer
   * @return dZ
   */
  Array2D relu_reverse(Array2D dA);

  /**
   * GD
   * Apply gradient descent to the weights and bias
   * @param Array
   * @param derivative
   * @return Updated Array
   */
  Array2D gd(Array2D X, Array2D dX);

public:
  Layer(size_t height, size_t width, double eta, bool last_layer = false);

  Array2D forward(Array2D A);
  
  Array2D reverse(Array2D dA);

};

#endif // __LAYER_H__
