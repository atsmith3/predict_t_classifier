#include "array.h"

#include <algorithm>
#include <cstdint>
#include <string>
#include <iostream>
#include <ostream>
#include <cassert>

Array2D::Array2D() {
  this->width = 0;
  this->height = 0;
}

Array2D::Array2D(size_t height, size_t width, double init) {
  this->width = width;
  this->height = height;
  data.resize(height);
  for(size_t y = 0; y < data.size(); y++) {
    data[y].resize(width);
  }
  for(size_t y = 0; y < height; y++) {
    for(size_t x = 0; x < width; x++) {
      data[y][x] = (((double)(random()%1000000)/1000000.0)*2.0 - 1.0)*init;
    }
  }
}

Array2D
Array2D::operator*(const Array2D& other) {
  assert(this->width == other.height);
  Array2D a = Array2D(this->height, other.width, 0.0);

  for(size_t i = 0; i < this->height; i++) {
    for(size_t j = 0; j < other.width; j++) {
      for(size_t k = 0; k < this->width; k++) {
        a.data[i][j] += this->data[i][k]*other.data[k][j];// + b.data[j][0];
      }
    }
  } 
  return a;
}

Array2D
Array2D::operator*(const double& other) {
  Array2D a = Array2D(this->height, this->width, 0.0);

  for(size_t i = 0; i < this->height; i++) {
    for(size_t j = 0; j < this->width; j++) {
      a.data[i][j] = this->data[i][j]*other;
    }
  } 
  return a;
}

Array2D
Array2D::operator+(const Array2D& other) {
  assert(this->height == other.height);
  Array2D a = *this;

  for(size_t i = 0; i < this->height; i++) {
    for(size_t j = 0; j < this->width; j++) {
      a.data[i][j] += other.data[0][j];
    }
  } 
  return a;
}

Array2D
Array2D::operator-(const Array2D& other) {
  assert(this->height == other.height);
  assert(this->width == other.width);
  Array2D a = *this;

  for(size_t i = 0; i < this->height; i++) {
    for(size_t j = 0; j < this->width; j++) {
      a.data[i][j] -= other.data[i][j];
    }
  } 
  return a;
}

Array2D
Array2D::apply(double func(double)) {
  Array2D a = *this;

  for(size_t i = 0; i < this->height; i++) {
    for(size_t j = 0; j < this->width; j++) {
      a.data[i][j] += func(this->data[i][j]);
    }
  } 
  return a;
}
