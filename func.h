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
 */
double loss(const Array2D F, const Array2D y);

/**
 * cross_entropy
 * Computes the cross entropy
 */
Array2D cross_entropy(const Array2D F, const Array2D y);

#endif // __FUNC_H__
