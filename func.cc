#include "func.h"

#include <algorithm>
#include <cmath>
#include <random>


/**
 * ReLU
 * @param i Element of Array
 * @return relu(i)
 */
double relu(double i) {
  return std::max(0.0, i);
}


/**
 * loss
 * Compute the Loss
 * @param F Result from the output layer
 * @param y Labeled correct action
 * @return loss
 */
double loss(const Array2D F, const Array2D y) {
  /* Caclulate Loss */
  double loss = 0.0;
  double inner_sum = 0.0;
  for(size_t i = 0; i < y.height; i++) {
    inner_sum = 0.0;
    for(size_t k = 0; k < F.width; k++) {
      inner_sum += exp(F.data[i][k]);
    }
    loss += F.data[i][(int)y.data[i][0]] - log(inner_sum);
  }
  return loss*(-1.0/F.height);
}

/**
 * cross_entropy
 * Computes the cross entropy
 * @param F Result from the output layer
 * @param y Labeled correct action
 * @return derivative
 */
Array2D cross_entropy(const Array2D F, const Array2D y) {
  Array2D dF = F;
  double selector = 0.0;
  double inner_sum = 0.0;
  /* Calculate dF */
  for(size_t i = 0; i < dF.height; i++) {
    for(size_t j = 0; j < dF.width; j++) {
      inner_sum = 0.0;
      selector = 0.0;
      for(size_t k = 0; k < dF.width; k++) {
        inner_sum += exp(F.data[i][k]);
      }
      if(j == (size_t)y.data[i][0]) {
        selector = 1.0;
      }
      dF.data[i][j] = (-1.0/dF.height)*(selector - (exp(F.data[i][j])/inner_sum));
    }
  }
  return dF;
}


/**
 * argmax
 * returns the index of the maximal element of the array
 * @param F Result from the output layer
 * @return index
 */
int argmax(const Array2D F) {
  auto result = std::max_element(F.data[0].begin(), F.data[0].end());
  return std::distance(F.data[0].begin(), result);
}
