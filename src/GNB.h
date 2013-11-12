#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include "model.h"

using namespace std;

class GNB
{

private:
	//mu indexed by sensor then hashed by annotation
	vector<map<string, long double> > mu;
	vector<map<string, long double> > sigma;
public:
	void computeMu(vector<person> & data);
	void computeSigma(vector<person> & data);

};