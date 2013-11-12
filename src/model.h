#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>

using namespace std;

struct session {
	// 0-8 -> sensors1-9, 9 -> annotation
	vector<double*> sensorData;
	int sid;
};

struct person {
	bool handedness;	// 0: left, 1: right
	int age;
	int uid;
	bool gender;		// 0: female, 1: male
	vector<session> sessions;
};

//reads data in from a file and places it in res.
//return value = 0 for success, 1 for failure
int readCSV(string file, vector<person> & res);