#ifndef MLP_H
#define MLP_H

class MLP {
private:
    int features;   // feature nodes
    int hidden;     // hidden nodes
    int output;     // output nodes
    double lrnRate; // learning rate
    double currentError;
    double **fhWeight;  // fhWeight[features][hidden]
    double **hoWeight;  // hoWeight[hidden][output]
    int function;
public:
    MLP(int f, int h, int o, double l, int func);
    void initialize();
    double train(double *x, int correctIdx);
	int classify(double *x);
    double func(double x);
    double deriv(double x);
};
#endif