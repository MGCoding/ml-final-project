#include <iostream>
#include "GNB.h"

using namespace std;

int main()
{
	vector<person> training, testing;
	
	cout << "Reading training..." << endl;
	readCSV("data/train.txt", training);
	cout << "Reading testing..." << endl;
	readCSV("data/train.txt", testing);
	
	GNB * gnb = new GNB();
	cout << "Computing mean..." << endl;
	gnb->computeMu(training);
	cout << "Computing variance..." << endl;
	gnb->computeSigma(training);
	
	int correct = 0, total = 0;
	cout << "Computing accuracy..." << endl;
	for(int i = 0; i < testing.size(); i++)
	{
		for(int k = 0; k < testing[i].sessions.size(); k++)
		{
			for(int j = 0; j < testing[i].sessions[k].sensorData.size(); j++)
			{
				string anno = gnb->computeAnnotation(testing[i].sessions[k].sensorData[j]);
				total++;
				if(atoi(anno.c_str()) == (int)testing[i].sessions[k].sensorData[j][9])
					correct++;
				//cout << "anno: " << anno << " " << (int)testing[i].sessions[k].sensorData[j][9] << endl;
				cout << " " << correct << "/" << total << "\r";
				cout.flush();
			}
		}
	}
	
	cout << "Accuracy: " << correct << "/" << total << " : " << (double)correct/(double)total << endl;
	
	return 0;
}