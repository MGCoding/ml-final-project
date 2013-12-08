#include "kNN.h"

inline string doubleToString(double value)
{
	ostringstream oss;
	oss << value;
	return oss.str();
}

kNN::kNN(int k)
{
	this->k = k;
}

void kNN::convertData(vector<person> & res)
{
	int p_size = res.size(), s_size;
	for(int i = 0; i < p_size; i++)
	{
		s_size = res[i].sessions.size();
		for(int j = 0; j < s_size; j++)
		{
			for(int k = 0; k < res[i].sessions[j].sensorData.size(); k++)
			{
				string anno = doubleToString(res[i].sessions[j].sensorData[k][9]);
				data[anno].push_back(res[i].sessions[j].sensorData[k]);
			}
		}
	}
    
    /*for(int i = 0; i < 9; i++)
    {
        cout << i << ": " << mins[i] << " " << maxs[i] << endl;
    }*/
    
}

long double kNN::computeDistance(double * data1, double * data2)
{
	long double sum = 0;
	for(int i = 0; i < 9; i++)
	{
		sum += (data1[i] - data2[i]) * (data1[i] - data2[i]);
	}
	
	return sum;//sqrt(sum);
}

string kNN::computeAnnotation(double * reading)
{	
	vector<long double> closest;
	closest.resize(k, DBL_MAX);
	vector<string> annotations;
	annotations.resize(k, "0");
	
	long double distance;
	for(map<string, vector<double *> >::iterator it = data.begin(); it != data.end(); ++it)
	{
		for(int i = 0; i < it->second.size(); i++)
		{
			//compute the distance
			distance = computeDistance(reading, it->second[i]);
			
			//check if its smaller than the others
			for(int j = 0; j < k; j++)
			{
				if(closest[j] > distance)
				{
					closest.insert(closest.begin() + j, distance);
					closest.erase(closest.end()-1);
					annotations.insert(annotations.begin() + j, doubleToString(reading[9]));
					annotations.erase(annotations.end() - 1);
					break;
				}
			}
		}
	}
	
	map<string, int> counts;
	
	for(int i = 0; i < closest.size(); i++)
	{
		counts[annotations[i]] = 0;
	}
	
	for(int i = 0; i < closest.size(); i++)
	{
		counts[annotations[i]]++;
	}
	
	int count = 0;
	string anno = "0";
	
	for(map<string, int>::iterator it = counts.begin(); it != counts.end(); ++it)
	{
		if(it->second > count)
		{
			count = it->second;
			anno = it->first;
		}
	}
	
	return anno;
}