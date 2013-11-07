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

// argv[1] = trainingSet, argv[2] = testSet
int main(int argc, char *argv[]) {
	vector<person> trainingData;
	
	ifstream inFile;
	// open trainingSet
	inFile.open(argv[1]);
	// read in trainingSet
	string data;
	getline(inFile,data);
	
	int char1, char2, anno, sex, uid, sid;
	long long stime;
	double sensor;
	int currentId;
	
	while(true){
		inFile >> char1;
		if(inFile.eof())
			break;
		inFile >> char2 >> anno >> sex;
		double *sensorData = (double*)malloc(sizeof(double)*10);
		for(int i = 0; i < 9; i++) {
			inFile >> sensorData[i];
		}
		sensorData[9] = anno;
		inFile >> uid >> sid >> stime;
		cout << stime << endl;
		if(trainingData.size() > 0 && trainingData.back().uid == uid) {
			// if same person, then check if same session
			if(trainingData.back().sessions.back().sid == sid) {
				trainingData.back().sessions.back().sensorData.push_back(sensorData);
			} else {
				session s;
				s.sensorData.push_back(sensorData);
				s.sid = sid;
				trainingData.back().sessions.push_back(s);
			}
		} else {
			// else new person
			person p;
			p.handedness = char2;
			p.age = char1;
			p.uid = uid;
			p.gender = sex;
			
			session s;
			s.sensorData.push_back(sensorData);
			s.sid = sid;
			
			p.sessions.push_back(s);
			trainingData.push_back(p);
		}
	}
	cout << trainingData.size() << endl;
	return 0;
}