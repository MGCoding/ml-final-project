#include "DT.h"

inline string doubleToString(double value)
{
	ostringstream oss;
	oss << value;
	return oss.str();
}

DT::DT(int k)
{
	this->k = k;
	
	for(int i = 0; i < 9; i++)
	{
		mins[i] = DBL_MAX;
		maxs[i] = -DBL_MAX;
	}
}

void DT::ConvertData(vector<person> & res)
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
				data[anno].push_back(res[i].session[j].sensorData[k]);
				
				for(int l = 0; l < 9; l++)
				{
					if(res[i].sessions[j].sensorData[k][i] < mins[i])
						mins[i] = res[i].sessions[j].sensorData[k][i];
					
					if(res[i].sessions[j].sensorData[k][i] > maxs[i])
						maxs[i] = res[i].sessions[j].sensorData[k][i];
				}
			}
		}
	}
}

double DT::computeGain(vector<SensorRange *> & assignments, int new_sensor, double old_entropy)
{
	int total = 0;
	
	vector<double> splits;
	vector<SensorRange*> new_ranges;
	vector<double> entropies;
	vector<int> counts;
	int count;
	for(int i = 0; i < k; i++)
	{
		splits.push_back(mins[new_sensor] + (i / k+1) * (maxs[i] - mins[i]));
	}
	
	for(int i = 0; i < splits.size(); i++)
	{
		SensorRange * sr = new SensorRange();
		sr->sensor_index = new_sensor;
		if(i==0)
		{
			sensor_min = -DBL_MAX;
			sensor_max = (splits[i] + splits[i] + 1)/2;
		}else if(i == splits.size() -1)
		{
			sensor_min = (splits[i] + splits[i] -1)/2;
			sensor_max = DBL_MAX;
		}else
		{
			sensor_min = (splits[i] + splits[i -1])/2;
			sensor_max = (splits[i] + splits[i+1])/2;
		}
		new_ranges.push_back(sr);
	}
	
	for(int i = 0; i < new_ranges.size(); i++)
	{
		assignments.push_back(new_ranges[i]);
		
		entropies.push_back(computeEntropy(assignments, count));
		counts.push_back(count);
		total += count;
		
		assignments.pop_back();
	}
	
	double new_entropy = 0;
	
	for(int i = 0; i < counts.size(); i++)
	{
		new_entropy += ((double)counts[i]/total) * entropies[i];
	}
	
	return old_entropy - new_entropy
}

double DT::computeEntropy(vector<SensorRange *> & assignments, int & count)
{
	double entropy;
	map<string, int> counts;
	
	for(map<string, vector<double*> >::iterator it = data.begin(); it != data.end(); ++it)
	{
		counts[it->first] = 0;
		for(int i = 0; i < it->second.size(); i++)
		{
			for(int j = 0; j < assignments.size(); j++)
			{
				if(it->second[i][assignments[j]->sensor_index] <= assignments[j]->min || it->second[i][assignments[j]->sensor_index] > assignments[j]->max)
					break;
					
				if(j == assignments.size()-1)
				{
					counts[it->first]++;
				}
			}
		}
	}
	
	double total = 0;
	
	for(map<string, int>::iterator it = counts.begin(); it != counts.end(); ++it)
	{
		total += it->second;
	}
	
	for(map<string, int>::iterator it = counts.begin(); it != counts.end(); ++it)
	{
		entropy += -(it->second/total) * log(it->second/total)/log(2);
	}
	
	return entropy;
}

