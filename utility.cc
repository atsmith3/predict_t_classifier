#include "utility.h"
#include "class.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

void train(Classifier* classes, char* filename) {
    FILE* training_data = fopen(filename, "r");
    char input;
    double data[F_H][F_W];
    int id;
    int predicted;

    intialize_random(classes);

    for(int e = 0; e < EPOCHS; e++){
      double total = 0;
      double total_correct = 0;
      while(!feof(training_data)) {

          for(int y = 0; y < F_H; y++) {
              for(int x = 0; x < F_W; x++) {
                  if(x == F_W - 1) {
                      fscanf(training_data, "%c\n", &input);
                  }
                  else {
                      fscanf(training_data, "%c ", &input);
                  }
                  data[y][x] = (int)(input - '0');
              }
          }

          fscanf(training_data, "%c\n", &input);
          id = (int)(input - '0');
          total++;

          predicted = classify(classes, data);
          if(predicted != id){
              update_weights(classes, data, id, predicted);
          }
          else
            total_correct++;
      }
      rewind(training_data);
      cout<<"EPOCH :" << e << " = " << total_correct/total<<endl;
  }


}

void show_results(Classifier* classes, double total, double total_correct){

    cout<<endl<<"------------------ Test Results ------------------"<<endl;

    for(int i = 0; i < 10; i++){

        double rate = classes[i].correct/classes[i].total;
        cout<<"Class " <<i<<" success rate = "<<rate<<endl;
        cout<<endl;
        }
    cout<<"Total correctness = " << total_correct/total<<endl;

}

void test(Classifier* classes, char* filename) {
    FILE* test_data = fopen(filename, "r");
    char input;
    double data[F_H][F_W];
    int id;
    int predicted;
    double total = 0;
    double total_correct = 0;
    double confusion[10][10];
    for(int a = 0; a < 10; a++){
      for(int b = 0; b < 10; b++)
        confusion[a][b] = 0;
    }

      while(!feof(test_data)) {

          for(int y = 0; y < F_H; y++) {
              for(int x = 0; x < F_W; x++) {
                  if(x == F_W - 1) {
                      fscanf(test_data, "%c\n", &input);
                  }
                  else {
                      fscanf(test_data, "%c ", &input);
                  }
                  data[y][x] = (int)(input - '0');
              }
          }

          fscanf(test_data, "%c\n", &input);
          id = (int)(input - '0');
          total++;
          classes[id].total++;

          predicted = classify(classes, data);



          if(predicted == id){
            classes[predicted].correct += 1;
            total_correct++;
          }
          else
            confusion[id][predicted]++;
      }


  show_results(classes, total, total_correct);
}
