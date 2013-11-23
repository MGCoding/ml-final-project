//
//  DT-test.cpp
//  
//
//  Created by TCS Lab on 11/23/13.
//  Copyright (c) 2013 Unviersity of Texas at Dallas. All rights reserved.
//

#include <iostream>
#include "DT.h"

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
    readCSV("data/train.txt", training);
    
    
    DT * dt = new DT(3);
    
    dt->convertData(training);
    cout << "here" << endl;
    dt->buildTree();
    cout << "here" << endl;
    int correct, total;
    correct = total = 0;
    readCSV("data/testdata.txt", testing);
    for(int i = 0; i < testing.size(); i++)
    {
        for(int j = 0; j < testing[i].sessions.size(); j++)
        {
            for(int k = 0; k < testing[i].sessions[j].sensorData.size(); k++)
            {
                total++;
                string anno = dt->classify(testing[i].sessions[j].sensorData[k]);
                if(anno == doubleToString(testing[i].sessions[j].sensorData[k][9]))
                   correct++;
                cout << correct << "/" << total << "\r";
                cout.flush();
            }
        }
    }
    
    cout << "Accuracy: " << correct/(double)total << endl;
    
    return 0;
}