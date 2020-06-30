
#include "class.h"

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

Classifier::Classifier() {
  total = 0;
  num = 0;
  correct = 0;
  for (int i = 0; i < F_W; i++) {
    for (int j = 0; j < F_H; j++) {
      weights[i][j] = 0;
    }
  }
}
