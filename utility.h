#ifndef UTILITY_H
#define UTILITY_H

#include "class.h"
#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void train(Classifier* classes, char* filename);
void show_results(Classifier* classes, double total, double total_correct);
void test(Classifier* classes, char* filename);
#endif
