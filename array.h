#ifndef __ARRAY2D_H__
#define __ARRAY2D_H__

#include <vector>
#include <iostream>
#include <ostream>

/**
 * Class Array2D
 *   2Dimensional Array Class 
 */
class Array2D {
public:
  std::vector<std::vector<double>> data;
  size_t width;
  size_t height;

  Array2D();
  Array2D(size_t height, size_t width, double init = 0.01);

  /**
   * operator+(const Array& other)
   * Add Array and bias
   * Return Array
   */
  Array2D operator+(const Array2D& other);

  /**
   * operator*(const Array2D& other)
   * Multiply two arrays
   * Return Array2D
   */
  Array2D operator*(const Array2D& other);

  /**
   * operator*(const double& other)
   * Multiply array by scalar
   * Return Array2D
   */
  Array2D operator*(const double& other);

  /**
   * operator-(const Array& other)
   * subtract two arrays for gradient descent
   * Return Array
   */
  Array2D operator-(const Array2D& other);

  /**
   * apply
   * Apply a function to the array element-wise
   * @return Array2D with applied function
   */
  Array2D apply(double func(double)); 

  /**
   * operator<<
   * Print out the array in a nice format
   * @param ostream
   * @param arr Array to print
   * @return ostream
   */
  friend std::ostream& operator<<(std::ostream& out, const Array2D& arr) {
    for(auto i : arr.data) {
      for(auto j : i) {
        out << j << " ";
      }
      out << "\n";
    }
    return out;
  }
};

#endif // __ARRAY2D_H__
