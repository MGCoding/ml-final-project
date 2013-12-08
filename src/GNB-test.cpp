#include <iostream>
#include "GNB.h"

using namespace std;

inline string doubleToString(double value)
{
	ostringstream oss;
	oss << value;
	return oss.str();
}

int main()
{
	vector<person> training, testing;
	
	cout << "Reading training..." << endl;
	readCSV("data/train.txt", training);
	cout << "Reading testing..." << endl;
	readCSV("data/testdata.txt", testing);
	long t = time(NULL);
	GNB * gnb = new GNB();
	cout << "Computing priors..." << endl;
	gnb->computePriors(training);
	cout << "Computing mean..." << endl;
	gnb->computeMu(training);
	cout << "Computing variance..." << endl;
	gnb->computeSigma(training);
	cout << "train time: " <<  (time(NULL) - t) << endl;
	int correct = 0, total = 0;
	cout << "Computing accuracy..." << endl;
	t = time(NULL);
	for(int i = 0; i < testing.size(); i++)
	{
		for(int k = 0; k < testing[i].sessions.size(); k++)
		{
			for(int j = 0; j < testing[i].sessions[k].sensorData.size(); j++)
			{
				string anno = gnb->computeAnnotation(testing[i].sessions[k].sensorData[j]);
				total++;
				if(anno == doubleToString(testing[i].sessions[k].sensorData[j][9]))
					correct++;
				//cout << "anno: " << anno << " " << (int)testing[i].sessions[k].sensorData[j][9] << endl;
				cout << " " << correct << "/" << total << "\r";
				cout.flush();
			}
		}
	}
	
	cout << "Accuracy: " << correct << "/" << total << " : " << (double)correct/(double)total << endl;
	cout << "test time: " << (time(NULL) - t) << endl;
	return 0;
}