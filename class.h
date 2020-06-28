#ifndef CLASS_H
#define CLASS_H

#include <vector>
#include <algorithm>

#define NUM_FEATURES       1024
#define F_W                32
#define F_H                32
#define ETA                100
#define EPOCHS             25
#define BIAS               1
#define RAND_RANGE         10

class Classifier {
 public:
    double weights[F_H][F_W];
    double total;
    int num;
    double correct;
    Classifier();

};



#endif
