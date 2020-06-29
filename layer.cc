#include "layer.h"
#include "func.h"

#include <algorithm>
#include <cstdint>

/**
 * affine_forward
 * compute Z = (A x W) + b
 * @param A Input Array
 * @return Z
 */
Array2D
Layer::affine_forward(Array2D A) {
  Z = (A*W) + b;
  return Z;
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
	/* dA Calculation */
	dA = dZ*dW;

  /* dW calculation */
	dW = dZ*A;

  /* db calculation */
  for(size_t j = 0; j < dZ.width; j++) {
    db.data[j][0] = 0.0;
    for(size_t i = 0; i < dZ.height; i++) {
      db.data[j][0] += dZ.data[i][j];
    }
  } 
  return dA;
}

/**
 * relu_reverse
 * Reverse activation function
 * @param dA output from previous layer
 * @return dZ
 */
Array2D
Layer::relu_reverse(Array2D dA) {
  for(size_t i = 0; i < Z.height; i++) {
    for(size_t j = 0; j < Z.width; j++) {
      if(std::max(0.0, Z.data[i][j]) > 0.0) {
        dZ.data[i][j] = dA.data[i][j];
      }
      else {
        dZ.data[i][j] = 0.0;
      }
    }
  }
  return dZ;
}

/**
 * GD
 * Apply gradient descent to the weights and bias
 * @param Array
 * @param derivative
 * @return Updated Array
 */
Array2D Layer::gd(Array2D X, Array2D dX) {
  return X - (dX*eta);
}

Layer::Layer(size_t height, size_t width, double eta, bool last_layer) {
  W = Array2D(height, width);
  b = Array2D(height, 1);

  this->eta = eta;
  this->last_layer = last_layer;
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
