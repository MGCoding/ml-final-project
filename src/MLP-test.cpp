#include "MLP.h"
#include "model.h"
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

// ./mlp training.txt testing.txt numIterations numHiddenNodes activationfunction

int main(int argc, char *argv[]) {
    if (argc != 6) {
        cout << "./mlp <trainfile> <testfile> <numIterations> <numHiddenNodes> <func:0-exp,1-tanh>" << endl;
        return 0;
    }
    
    vector<person> training;
    cout << "Reading in training data..." << endl;
    readCSV(argv[1],training);
    // grab all of the sensor data
    vector<double*> trainingData;
    map<double,int> annotationIdx;
    map<double,bool> annotation;
    for (int i = 0; i < training.size(); i++) {
        for (int j = 0; j < training[i].sessions.size(); j++) {
            for (int k = 0; k < training[i].sessions[j].sensorData.size(); k++) {
                if(training[i].sessions[j].sensorData[k][9] != 0) {
                    trainingData.push_back(training[i].sessions[j].sensorData[k]);
                    annotation[training[i].sessions[j].sensorData[k][9]] = true;
                }
            }
        }
    }
    
    int i = 0;
    double *idxAnnotation = new double[annotation.size()];
    for(map<double,bool>::iterator it = annotation.begin(); it != annotation.end(); it++, i++) {
        annotationIdx[it->first] = i;
        idxAnnotation[i] = it->first;
    }
    
    vector<person> testing;
    cout << "Reading in testing data..." << endl;
    readCSV(argv[2],testing);
    vector<double*> testingData;
    for (int i = 0; i < testing.size(); i++) {
        for (int j = 0; j < testing[i].sessions.size(); j++) {
            for (int k = 0; k < testing[i].sessions[j].sensorData.size(); k++) {
                if(testing[i].sessions[j].sensorData[k][9] != 0)
                    testingData.push_back(testing[i].sessions[j].sensorData[k]);
            }
        }
    }
    
    cout << "Creating MLP..." << endl;
    MLP *m = new MLP(9,atoi(argv[4]),annotationIdx.size(),0.1,atoi(argv[5]));
    double error;
    
    cout << "Training MLP..." << endl;
    for(int epoch = 0; epoch < atoi(argv[3]); epoch++) {
        cout << epoch << "...";
        cout.flush();
        for (int i = 0; i < trainingData.size(); i++) {
            error = m->train(trainingData[i],annotationIdx[trainingData[i][9]]);
        }
        random_shuffle(trainingData.begin(),trainingData.end());
    }
    cout << endl;
    cout << "Calculating accuracy..." << endl;
    double correct = 0, total = 0;
    for (int i = 0; i < testingData.size(); i++)
    {
        if(idxAnnotation[m->classify(testingData[i])]==testingData[i][9])
            correct++;
        total++;
    }
    cout << correct << ", " << total << ", " << correct/total << endl;
    return 0;
}