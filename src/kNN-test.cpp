#include <iostream>
#include <pthread.h>
#include "kNN.h"

using namespace std;

inline string doubleToString(double value)
{
	ostringstream oss;
	oss << value;
	return oss.str();
}

kNN * knn;
vector<person> testing;

int main()
{
	vector<person> training;
	cout << "Reading training data" << endl;
    readCSV("data/train.txt", training);
	
	knn = new kNN(3);
	
	cout << "Converting training data" << endl;
	knn->convertData(training);
	
	cout << "Reading test data" << endl;
	readCSV("data/testdata.txt", testing);
	
	int correct, total;
    correct = total = 0;
	long t = time(NULL);
    for(int i = 0; i < testing.size(); i++)
    {
        for(int j = 0; j < testing[i].sessions.size(); j++)
        {
            for(int k = 0; k < testing[i].sessions[j].sensorData.size(); k++)
            {
                total++;
                string anno = knn->computeAnnotation(testing[i].sessions[j].sensorData[k]);
                if(anno == doubleToString(testing[i].sessions[j].sensorData[k][9]))
                   correct++;
                cout << correct << "/" << total << "\r";
                cout.flush();
            }
        }
    }
    
    cout << endl << "Accuracy: " << correct/(double)total << endl;
	cout << "testing time: " << (time(NULL) - t) << endl;
	return 0;
}