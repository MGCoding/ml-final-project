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

void DT::convertData(vector<person> & res)
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
				
                //cout << "here" << endl;
                
				for(int l = 0; l < 9; l++)
				{
					if(res[i].sessions[j].sensorData[k][l] < mins[l])
						mins[l] = res[i].sessions[j].sensorData[k][l];
					
					if(res[i].sessions[j].sensorData[k][l] > maxs[l])
						maxs[l] = res[i].sessions[j].sensorData[k][l];
				}
			}
		}
	}
    
    /*for(int i = 0; i < 9; i++)
    {
        cout << i << ": " << mins[i] << " " << maxs[i] << endl;
    }*/
    
}

double DT::computeGain(vector<SensorRange *> & assignments, int new_sensor, double old_entropy)
{
    //cout << "entering compute gain" << endl;
	int total = 0;
	
	vector<double> splits;
	vector<SensorRange*> new_ranges;
	vector<double> entropies;
	vector<int> counts;
	int count = 0;
	
	for(int i = 0; i < k; i++)
	{
		splits.push_back(mins[new_sensor] + i * ((maxs[new_sensor] - mins[new_sensor])/(k+1)));
        //cout << splits.back() << " ";
	}
    cout << endl;
	
	for(int i = 0; i < splits.size(); i++)
	{
		SensorRange * sr = new SensorRange();
		sr->sensor_index = new_sensor;
		if(i==0)
		{
			sr->min = -DBL_MAX;
			sr->max = (splits[i] + splits[i+1])/2;
		}else if(i == splits.size() -1)
		{
			sr->min = (splits[i] + splits[i-1])/2;
			sr->max = DBL_MAX;
		}else
		{
			sr->min = (splits[i] + splits[i-1])/2;
			sr->max = (splits[i] + splits[i+1])/2;
		}
		new_ranges.push_back(sr);
	}
	
	for(int i = 0; i < new_ranges.size(); i++)
	{
		assignments.push_back(new_ranges[i]);
		
		entropies.push_back(computeEntropy(assignments, count));
        //cout << "entropy: " << entropies.back() << endl;
		counts.push_back(count);
		total += count;
		
		assignments.pop_back();
	}
	
	double new_entropy = 0;
	
	for(int i = 0; i < counts.size(); i++)
	{
		new_entropy += ((double)counts[i]/total) * entropies[i];
	}
	
    //cout << "exiting compute gain" << endl;
    
	return old_entropy - new_entropy;
}

double DT::computeEntropy(vector<SensorRange *> & assignments, int & count)
{
    //cout << "entering compute entropy" << endl;

    for(int i = 0; i < assignments.size(); i++)
    {
        cout << assignments[i]->sensor_index << " ";
    }
    cout << endl;
	double entropy = 0;
	map<string, int> counts;
	bool inRange;
	for(map<string, vector<double*> >::iterator it = data.begin(); it != data.end(); ++it)
	{
		counts[it->first] = 0;
		for(int i = 0; i < it->second.size(); i++)
		{
            inRange = true;
			for(int j = 0; j < assignments.size(); j++)
			{
				if(it->second[i][assignments[j]->sensor_index] <= assignments[j]->min || it->second[i][assignments[j]->sensor_index] > assignments[j]->max)
				{
                    inRange = false;
                    break;
                }
			}
            if(inRange)
                counts[it->first]++;
		}
	}
	
	double total = 0;
	
	for(map<string, int>::iterator it = counts.begin(); it != counts.end(); ++it)
	{
		total += it->second;
	}
	
    if(total == 0)
    {
        //cout << "exiting compute entropy" << endl;
        return 0;
    }
	count = total;
	
	for(map<string, int>::iterator it = counts.begin(); it != counts.end(); ++it)
	{
        if(it->second != 0)
            entropy += -(it->second/total) * log(it->second/total)/log(2);
	}
    
    //cout << "exiting compute entropy" << endl;
	
	return entropy;
}

bool isInAssignments(vector<SensorRange*> & assignments, int sensor_id)
{
    for(int i = 0; i < assignments.size(); i++)
    {
        if(assignments[i]->sensor_index == sensor_id)
            return true;
    }
    
    return false;
}

void DT::buildTree()
{
    vector<SensorRange *> assignments;
    root = buildBranch(assignments, 1, k);
}

Node * DT::buildBranch(vector<SensorRange *> & assignments, double entropy, int k)
{
    map<string, int> counts;
    
    for(int i = 0; i < 9; i++)
    {
        if(!isInAssignments(assignments, i))
        {
            //cout << "assignments.size(): " << assignments.size() << endl;
            break;
        }else if(i == 8)
        {
            for(map<string, vector<double*> >::iterator it = data.begin(); it != data.end(); ++it)
            {
                for(int i = 0; i < it->second.size(); i++)
                {
                    for(int j = 0; j < assignments.size(); j++)
                    {
                        if(it->second[i][assignments[j]->sensor_index] <= assignments[j]->min || it->second[i][assignments[j]->sensor_index] > assignments[j]->max)
                            break;
                        
                        map<string, int>::iterator it2 = counts.find(it->first);
                        if(it2 == counts.end())
                        {
                            counts[it->first] = 0;
                        }else
                        {
                            counts[it->first]++;
                        }
                    }
                }
            }
            
            int max = -1;
            string annotation = "";
            
            for(map<string, int>::iterator it = counts.begin(); it != counts.end(); ++it)
            {
                if(it->second > max)
                {
                    max = it->second;
                    annotation = it->first;
                }
            }
            
            Node * child = new Node();
            child->annotation = annotation;
            child->type=2;
            return child;
        }
    }
    
    //check for all one annotation
    for(map<string, vector<double*> >::iterator it = data.begin(); it != data.end(); ++it)
	{
		for(int i = 0; i < it->second.size(); i++)
		{
			for(int j = 0; j < assignments.size(); j++)
			{
				if(it->second[i][assignments[j]->sensor_index] <= assignments[j]->min || it->second[i][assignments[j]->sensor_index] > assignments[j]->max)
					break;
                
                map<string, int>::iterator it2 = counts.find(it->first);
                if(it2 == counts.end())
                {
                    counts[it->first] = 0;
                }else
                {
                    counts[it->first]++;
                }
			}
		}
	}
    
    if(counts.size() == 1)
    {
        Node * child = new Node();
        child->annotation = counts.begin()->first;
        child->type = 2;
        return child;
    }else if(counts.size() == 0)
    {
        cout << "This shouldn't happen" << endl;
    }
    
    //determine which sensor to split on based on highest gain
    double max_gain = -DBL_MAX;
    double gain;
    int new_sensor_index = -1;
    for(int i = 0; i < 9; i++)
    {
        if(!isInAssignments(assignments, i))
        {
            gain = computeGain(assignments, i, entropy);
            //cout << "gain: " << gain << endl;
            if(gain > max_gain)
            {
                max_gain = gain;
                new_sensor_index = i;
            }
        }
    }
    
    if(new_sensor_index == -1)
    {
        int most_annotated = -1;
        string annotated = "";
        Node * child = new Node();
        child->type = 2;
        
        for(map<string, int>::iterator it = counts.begin(); it != counts.end(); ++it)
        {
            if(most_annotated < it->second)
            {
                most_annotated = it->second;
                annotated = it->first;
            }
        }
        
        child->annotation = annotated;
        return child;
    }
    
    //cout << "splitting on: " << new_sensor_index << endl;
    
    //alter assignments
    vector<double> splits;
	vector<SensorRange*> new_ranges;
	int count;
	
    for(int i = 0; i < k; i++)
	{
		splits.push_back(mins[new_sensor_index] + (i / k+1) * (maxs[new_sensor_index] - mins[new_sensor_index]));
	}
	
	for(int i = 0; i < splits.size(); i++)
	{
		SensorRange * sr = new SensorRange();
		sr->sensor_index = new_sensor_index;
		if(i==0)
		{
			sr->min = -DBL_MAX;
			sr->max = (splits[i] + splits[i+1])/2;
		}else if(i == splits.size() -1)
		{
			sr->min = (splits[i] + splits[i-1])/2;
			sr->max = DBL_MAX;
		}else
		{
			sr->min = (splits[i] + splits[i-1])/2;
			sr->max = (splits[i] + splits[i+1])/2;
		}
		new_ranges.push_back(sr);
	}
    
    Node * sensorNode = new Node();
    sensorNode->type = 0;
    sensorNode->sensor_index = new_sensor_index;
	
	for(int i = 0; i < new_ranges.size(); i++)
	{
		assignments.push_back(new_ranges[i]);
		
        double entropy1 = computeEntropy(assignments, count);
        Node * child = buildBranch(assignments, entropy1, k);
        sensorNode->children.push_back(child);
		sensorNode->splits.push_back(new_ranges[i]);
        
		assignments.pop_back();
	}
    
    return sensorNode;
}

string DT::internalClassify(Node * node, double * example)
{
    if(node->type == 0)
    {
        for(int i = 0; i < node->splits.size(); i++)
        {
            if(node->splits[i]->min <= example[node->sensor_index] && node->splits[i]->max > example[node->sensor_index])
            {
                return internalClassify(node->children[i], example);
            }
        }
    }
    
	return node->annotation;
}

string DT::classify(double * example)
{
    if(root->type == 0)
    {
        for(int i = 0; i < root->splits.size(); i++)
        {
            if(root->splits[i]->min <= example[root->sensor_index] && root->splits[i]->max > example[root->sensor_index])
            {
                return internalClassify(root->children[i], example);
            }
        }
    }
    
    return root->annotation;
}

