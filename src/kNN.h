#include <iostream>
#include <map>
#include <sstream>
#include <cmath>
#include <cfloat>
#include "model.h"

using namespace std;

class kNN {
private:
	map<string, vector<double*> > data;
	int k;
public:
	kNN(int k);
	void convertData(vector<person> & res);
	string computeAnnotation(double * reading);
	long double computeDistance(double * data1, double * data2);
};