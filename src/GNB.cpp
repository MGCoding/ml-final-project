#include "GNB.h"

inline string doubleToString(double value)
{
	ostringstream oss;
	oss << value;
	return oss.str();
}

void GNB::computeMu(vector<person> & data)
{
	for(int i = 0; i < 9; i++)
		mu.push_back(map<string, long double>());
	
	map<string, int> counts;
	int p_size = data.size(), s_size;
	
	//for each person in the training data
	for(int m = 0; m < p_size; m++)
	{
		//for each session in the training data
		s_size = data[m].sessions.size();
		for(int j = 0; j < s_size; j++)
		{
			for(int z = 0; z < data[m].sessions[j].sensorData.size(); z++)
			{
				string anno = doubleToString(data[m].sessions[j].sensorData[z][9]);
				map<string,int>::iterator it = counts.find(anno);

				if(it == counts.end())
				{
					counts[anno] = 1;
				}else{
					counts[anno]++;
				}

				for(int i = 0; i < 9; i++)
				{
					
					map<string,long double>::iterator it2 = mu[i].find(anno);
					if(it2 == mu[i].end())
					{
						mu[i][anno] = data[m].sessions[j].sensorData[z][i];
					}else
					{
						mu[i][anno] += data[m].sessions[j].sensorData[z][i];
					}
				}
			}
		}
	}
	
	//divide by counts
	for(int i = 0; i < 9; i++)
	{
		for(map<string,long double>::iterator it = mu[i].begin(); it != mu[i].end(); ++it)
		{
			it->second /= counts[it->first];
		}
	}
}

void GNB::computeSigma(vector<person> & data)
{
	for(int i = 0; i < 9; i++)
		sigma.push_back(map<string, long double>());
		
	map<string, int> counts;
	int p_size = data.size(), s_size;
	
	//for each person in the training data
	for(int m = 0; m < p_size; m++)
	{
		//for each session in the training data
		s_size = data[m].sessions.size();
		for(int j = 0; j < s_size; j++)
		{
			for(int z = 0; z < data[m].sessions[j].sensorData.size(); z++)
			{
				string anno = doubleToString(data[m].sessions[j].sensorData[z][9]);
				map<string,int>::iterator it = counts.find(anno);

				if(it == counts.end())
				{
					counts[anno] = 1;
				}else{
					counts[anno]++;
				}

				for(int i = 0; i < 9; i++)
				{
					
					map<string,long double>::iterator it2 = sigma[i].find(anno);
					if(it2 == sigma[i].end())
					{
						sigma[i][anno] = (data[m].sessions[j].sensorData[z][i] - mu[i][anno]) * (data[m].sessions[j].sensorData[z][i] - mu[i][anno]);
					}else
					{
						sigma[i][anno] += (data[m].sessions[j].sensorData[z][i] - mu[i][anno]) * (data[m].sessions[j].sensorData[z][i] - mu[i][anno]);
					}
				}
			}
		}
	}
	
	//divide by counts
	for(int i = 0; i < 9; i++)
	{
		for(map<string,long double>::iterator it = sigma[i].begin(); it != sigma[i].end(); ++it)
		{
			it->second /= counts[it->first]-1;
			//take sqrt?
		}
	}
}

string GNB::computeAnnotation(double * sensorData)
{
	double first = 0, second = 0;
	string anno = "";
	long double max = -DBL_MAX;
	for(int i = 0; i < 9; i++)
	{
		for(map<string, long double>::iterator it = sigma[i].begin(); it != sigma[i].end(); ++it)
		{
			first = sqrt(it->second) * SQRT_2PI;
			second = 1/(2 * it->second);
			second *= -1 * ((sensorData[i] - mu[i][it->first]) * (sensorData[i] - mu[i][it->first]));
			first = -1 * log(first);
			first += second;
			
			if(first > max)
			{
				anno = it->first;
				max = first;
			}
		}
	}

	return anno;
}