#include "MLP.h"
#include <iostream>
#include <cfloat>
#include <time.h>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>

MLP::MLP(int f, int h, int o, double l, int func) {
  features = f;
  hidden = h;
  output = o;
  lrnRate = l;
  function = func;

  srand(time(NULL));
  fhWeight = new double*[features];
  for (int i = 0; i < features; i++) {
    fhWeight[i] = new double[hidden];
    for (int j = 0; j < hidden; j++) {
      fhWeight[i][j] = ((double)(rand()%2000)-1000)/1000;
    }
  }
  hoWeight = new double*[hidden];
  for (int i = 0; i < hidden; i++) {
    hoWeight[i] = new double[output];
    for (int j = 0; j < output; j++) {
      hoWeight[i][j] = ((double)(rand()%2000)-1000)/1000;
    }
  }
}

void MLP::initialize() {
  
}

double MLP::func(double x) {
  double v = 0;
  switch(function) {
    case 0:
      v = 1/(1+exp(-x));
      break;
    case 1:
      v = tanh(x);
      break;
    default:
      break;
  }
  return v;
}

double MLP::deriv(double x) {
  double v = 0;
  switch(function) {
    case 0:
      v = x * (1 - x);
      break;
    case 1:
      v = 1 - tanh(x)*tanh(x);
      break;
    default:
      break;
  }
  return v;
}

double MLP::train(double *x, int correctIdx) {
  double *hiddenVal = new double[hidden];
  double *outputVal = new double[output];
  
  // present a training pattern and obtain the output
  // send weight through feature nodes to hidden nodes
  for (int i = 0; i < hidden; i++) {
    hiddenVal[i] = 0;
    for (int j = 0; j < features; j++) {
      hiddenVal[i] += fhWeight[j][i]*x[j];
    }
    hiddenVal[i] = func(hiddenVal[i]);
  }
  // send weight through hidden nodes to output nodes
  for (int i = 0; i < output; i++) {
    outputVal[i] = 0;
    for (int j = 0; j < hidden; j++) {
      outputVal[i] += hoWeight[j][i]*hiddenVal[j];
    }
    outputVal[i] = func(outputVal[i]);
  }
  int maxIdx = -1;
  double maxVal = -DBL_MAX;
  for (int i = 0; i < output; i++) {
    if (outputVal[i] > maxVal) {
      maxVal = outputVal[i];
      maxIdx = i;
    }
  }
  if (maxIdx != correctIdx) {
    // back propogate
    // correct the output layer of weights
    double *delta = new double[output];
    for (int h = 0; h < hidden; h++) {
      for (int o = 0; o < output; o++) {
	double target = 0;
	if (correctIdx == o) {
	  target = 1;
	}
	target -= outputVal[o];
	delta[o] = deriv(outputVal[o]) * target;
	hoWeight[h][o] += lrnRate * delta[o] * outputVal[h];
      }
    }
    // correct the input weights
    for (int i = 0; i < features; i++) {
      for (int h = 0; h < hidden; h++) {
	double deltah = 0;
	for (int o = 0; o < output; o++) {
	  deltah += delta[o]*hoWeight[h][o];
	}
	deltah *= deriv(hiddenVal[h]);
	fhWeight[i][h] += lrnRate * deltah * hiddenVal[i];
      }
    }
    delete [] delta;
  }
  double error = 0;
  for (int n = 0; n < output; n++) {
    if (maxIdx == n) {
      error += (1-outputVal[n])*(1-outputVal[n]);
    } else {
      error += (0-outputVal[n])*(0-outputVal[n]);
    }
  }
  error = sqrt(error)/output;
  //cout << "Error: " << error << endl;
  // clean up
  delete [] hiddenVal;
  delete [] outputVal;
  return error;
}

int MLP::classify(double *x) {
  double *hiddenVal = new double[hidden];
  double *outputVal = new double[output];
  
  for (int i = 0; i < hidden; i++) {
    hiddenVal[i] = 0;
    for (int j = 0; j < features; j++) {
      hiddenVal[i] += fhWeight[j][i]*x[j];
    }
    hiddenVal[i] = func(hiddenVal[i]);
  }
  for (int i = 0; i < output; i++) {
    outputVal[i] = 0;
    for (int j = 0; j < hidden; j++) {
      outputVal[i] += hoWeight[j][i]*hiddenVal[j];
    }
    outputVal[i] = func(outputVal[i]);
  }
  int maxIdx = -1;
  double maxVal = -DBL_MAX;
  for (int i = 0; i < output; i++) {
    if (outputVal[i] > maxVal) {
      maxVal = outputVal[i];
      maxIdx = i;
    }
  }
  delete [] hiddenVal;
  delete [] outputVal;
  return maxIdx;
}