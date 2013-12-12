#ifndef MCP_H
#define MCP_H

#include <iostream>
#include <vector>
#include <map>
#include <time.h>
#include "model.h"

using namespace std;

class MCP{
    
private:
    vector<double*> x;  // x[i][j] jth sensor of ith example
    map<double,double*> w;  // w[i][j] is the jth weight (0-8) of the ith annotation
    vector<double> c;   // class that example x belongs to
    vector<double> classes;
    double lrnRate;     // learning rate for perceptron algorithm
    int size;
    
    double dotProd(double *w, double *x);
    void sub(double *w, double *x);
    void add(double *w, double *x);
    double getAttribute(double *x);
public:
    MCP();
    void getTrainingData(vector<person> &data);
    void perceptronAlgo(int iterations);
    void evalAccuracy(vector<person> &data);
};

#endif