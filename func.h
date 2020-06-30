#ifndef __FUNC_H__
#define __FUNC_H__

#include "array.h"

/**
 * ReLU
 * @param i Element of Array
 * @return relu(i)
 */
double relu(double i);

/**
 * loss
 * Compute the Loss
 * @param F Result from the output layer
 * @param y Labeled correct action
 * @return loss
 */
double loss(const Array2D F, const Array2D y);

/**
 * cross_entropy
 * Computes the cross entropy
 * @param F Result from the output layer
 * @param y Labeled correct action
 * @return derivative
 */
Array2D cross_entropy(const Array2D F, const Array2D y);

/**
 * argmax
 * returns the index of the maximal element of the array
 * @param F Result from the output layer
 * @return index
 */
int argmax(const Array2D F);

#endif // __FUNC_H__
