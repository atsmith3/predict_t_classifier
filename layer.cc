#include "layer.h"
#include "func.h"

#include <algorithm>
#include <cstdint>

/**
 * ReLU
 * @param i Element of Array
 * @return relu(i)
 */
double relu(double i) {
  return std::max(0.0, i);
}

/**
 * affine_forward
 * compute Z = (A x W) + b
 * @param A Input Array
 * @return Z
 */
Array2D
Layer::affine_forward(Array2D A) {
  Z = (A*W) + b;
}

/**
 * relu_forward
 * apply relu to Z
 * @param Z Input array
 * @return ReLU(Z)
 */
Array2D 
Layer::relu_forward(Array2D Z) {
  return Z.apply(relu);
}

/**
 * affine_reverse
 * Compute Gradient with respect to input
 * @param dZ output from relu_reverse
 * @return dA
 */
Array2D
Layer::affine_reverse(Array2D dZ) {

}

/**
 * relu_reverse
 * Reverse activation function
 * @param dA output from previous layer
 * @return dZ
 */
Array2D 
Layer::relu_reverse(Array2D dA) {

}

/**
 * GD
 * Apply gradient descent to the weights and bias
 * @param Array
 * @param derivative
 * @return Updated Array
 */
Array2D gd(Array2D X, Array2D dX) {
  return X - (eta*dX);
}

Layer::Layer(size_t height, size_t width, double eta, bool last_layer = false) {
  W = Array2D(height, width);
  b = Array2D(height, 1);
}

Array2D
Layer::forward(Array2D A) {
  // Cache
  dA = A;
  dW = W;
  db = b;

  // Affine Forward
  Z = affine_forward(A);

  // Cache
  dZ = Z;

  // ReLU Forward
  if(!last_layer) {
    return relu_forward(Z);
  }
  return Z;
}

Array2D
Layer::reverse(Array2D dA) {
  dZ = dA;
  // ReLU Reverse
  if(!last_layer) {
    dZ = relu_reverse(dA);
  }

  // Reverse Affine
  Array2D dAn = affine_reverse(dZ);

  // Gradient Descent
  W = gd(W, dW);
  b = gd(b, db);

  return dAn;
}
