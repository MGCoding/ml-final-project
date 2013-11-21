#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <cmath>
#include <float.h>
#include "model.h"

using namespace std;

struct SensorRange{
	int sensor_index;
	double min; //greater than
	double max;	//less than or equal to
};

struct Node{
	SensorRange * s_range;
	vector<Node> children;
};

class DT {

private:
	//annotation, data
	map<string, vector<double *> > data;
	int k;
	double mins[9];
	double maxs[9];
	
public:
	DT(int k);
	void convertData(vector<person> & res);
	double computeEntropy(vector<SensorRange *> & assignments, int & count);
	double computeGain(vector<SensorRange *> & assignments);
	void BuildTree(Node * parent, vector<SensorRange *> & assignments, double entropy, int k);
};