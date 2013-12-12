#include <iostream>
#include "MCP.h"

using namespace std;

// ./mcp <trainfile> <testfile> <num-iterations>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cout << "./mcp <trainfile> <testfile> <num-iterations>" << endl;
        return 0;
    }
    
    vector<person> training, testing;
	cout << "Reading in training data..." << endl;
    readCSV(argv[1],training);
    
    MCP *mcp = new MCP();
    mcp->getTrainingData(training);
    mcp->perceptronAlgo(atoi(argv[3]));
    
    cout << "Reading in testing data..." << endl;
    readCSV(argv[2],testing);
    mcp->evalAccuracy(testing);
    
    return 0;
}