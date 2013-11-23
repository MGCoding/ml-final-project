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
    int type; //0 = sensor_index, 1 = split, 2 = annotation
    int sensor_index;
	vector<SensorRange *> splits;
	vector<Node *> children;
    string annotation;
};

class DT {

private:
	//annotation, data
	map<string, vector<double *> > data;
	int k;
	double mins[9];
	double maxs[9];
	Node * root;
public:
	DT(int k);
	void convertData(vector<person> & res);
	double computeEntropy(vector<SensorRange *> & assignments, int & count);
	double computeGain(vector<SensorRange *> & assignments, int new_sensor, double old_entropy);
    void buildTree();
	Node * buildBranch(vector<SensorRange *> & assignments, double entropy, int k);
    string classify(double* example);
    string internalClassify(Node * node, double * example);
};