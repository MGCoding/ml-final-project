#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <cmath>
#include <float.h>
#include "model.h"

using namespace std;

class GNB
{

private:
	//mu indexed by sensor then hashed by annotation
	vector<map<string, long double> > mu;
	vector<map<string, long double> > sigma;
	static const double SQRT_2PI = 2.50662827463;
public:
	void computeMu(vector<person> & data);
	void computeSigma(vector<person> & data);
	string computeAnnotation(double * sensorData);
};