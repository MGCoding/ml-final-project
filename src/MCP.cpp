#include "MCP.h"
#include <cfloat>

MCP::MCP() {
    lrnRate = 0.01;
    size = 9;
}

double MCP::dotProd(double *wj, double *x) {
    double prod = 0;
    for (int i = 0; i < size; i++) {
        prod += wj[i] * x[i];
    }
    return prod;
}

void MCP::sub(double *wj, double *x) {
    for (int i = 0; i < size; i++)
        wj[i] -= lrnRate * x[i];
}

void MCP::add(double *wj, double *x) {
    for (int i = 0; i < size; i++)
        wj[i] += lrnRate * x[i];
}

void MCP::getTrainingData(vector<person> &data) {
    map<double,bool> temp_class;
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].sessions.size(); j++) {
            for (int k = 0; k < data[i].sessions[j].sensorData.size(); k++) {
                x.push_back(data[i].sessions[j].sensorData[k]);
                c.push_back(data[i].sessions[j].sensorData[k][9]);
                temp_class[c.back()] = true;
            }
        }
    }
    for (map<double,bool>::iterator it = temp_class.begin(); it != temp_class.end(); it++) {
        classes.push_back(it->first);
    }
    cout << x.size() << endl;
}

void MCP::perceptronAlgo(int iterations) {
    // randomize weights
    cout << "Randomizing weights..." << endl;
    srand(time(NULL));
    for (int i = 0; i < classes.size(); i++) {
        double *wj = new double[9];
        for (int j = 0; j < 9; j++) {
            wj[j] = (double)(rand()%1000)/2000;
        }
        w[classes[i]] = wj;
    }
    
    //cout << w.size() << endl;
    cout << "Iteration over weights..." << endl;
    for (int itr = 0; itr < iterations; itr++) {
      cout << itr << "...";
      cout.flush();
        for (int xIdx = 0; xIdx < x.size(); xIdx++) {
            double i = c[xIdx];
            double g = dotProd(w[i],x[xIdx]);
            bool error = false;
            
            for (int j = 0; j < classes.size(); j++) {
                if (classes[j] != i && g < dotProd(w[classes[j]],x[xIdx])) {
                    error = true;
                    sub(w[classes[j]],x[xIdx]);
                }
            }
            
            if (error) {
                add(w[i],x[xIdx]);
            }
        }
    }
}

double MCP::getAttribute(double *x) {
  double max = -DBL_MAX;
  int maxIdx = 0;
  for(int i = 0; i < classes.size(); i++) {
    double prod = dotProd(w[classes[i]],x);
    if(prod > max) {
      max = prod;
      maxIdx = i;
    }
  }
  return classes[maxIdx];
}

void MCP::evalAccuracy(vector<person> &data) {
  cout << "Evaluating accuracy..." << endl;
  double total = 0, correct = 0;
  for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].sessions.size(); j++) {
            for (int k = 0; k < data[i].sessions[j].sensorData.size(); k++) {
	        total++;
		if(data[i].sessions[j].sensorData[k][9] == getAttribute(data[i].sessions[j].sensorData[k]))
		  correct++;
		cout << " " << correct << "/" << total << "\r";
		cout.flush();
            }
        }
    }
    cout << correct << "/" << total << " = " << correct/total << endl;
}