#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string> 
#include <cmath>
#include <algorithm>
using namespace std;


/*
1. The nearest neighbor classifier (All you need to do is to keep the training instances in memory and
when a new data point is given for classification, compute its distance to the training points and return
the class label of the nearest training point)

2. The Leave-one-out-validatior: It uses the training data and the classifier. Given a subset of features
as input, it returns an accuracy score as output.

3. The search algorithm: It starts from an initial state (in forward selection, it is an empty set and in
backward elimination, it is the set of all features), adds/removes features to/from the previous set and
evaluates the resulting feature subsets using the validator (assigns an accuracy score to them) and
chooses the highest-accuracy option and continues until no more features can be added/removed. At
the end, it reports the feature subset with highest accuracy as the best feature subset


***find some code to load the data into your program (ASCII Text, IEEE standard for 8 place floating numbers)
*/



//Vector for storing Data
vector< vector < double > >  TrainingDataSet;
void printInitial()
{
	for(unsigned i = 0; i < TrainingDataSet.size(); i++)
	{
		for(unsigned k = 0; k < TrainingDataSet[i].size(); k++)
		{
			cout << TrainingDataSet[i][k] << ' ';
		}
		cout << endl;
	}
}

void print(vector<int> SetofIndex, float Percentage)
{
	cout << "Using feature set: ";
	for(unsigned i = 0; i < SetofIndex.size(); i++)
	{
		cout << SetofIndex[i] << ' ';
	}
	cout << "accuracy of " << Percentage << endl;
	
}


void printFinal(vector<int> SetofIndex, float Percentage)
{
	cout << "The best accuracy is gained using feature set: ";
	for(unsigned i = 0; i < SetofIndex.size(); i++)
	{
		cout << SetofIndex[i] << ' ';
	}
	cout << "accuracy of " << Percentage << endl;
	
}

//assume works
bool Read(string NameOfFile)
{	
	ifstream fin;
    fin.open(NameOfFile.c_str());
    if(!fin.is_open())
    {
        cout << "Error opening file";
        return false;
    }
           
    string temp; 
    string DoublePart;
    string ExponentPart;
    string PlusOrMinus;
    vector <double> TempVect;
   
    while(fin >> temp)
    {
    	TempVect.push_back(stod(temp));
    	if(fin.peek() == 13)
    	{
    		TrainingDataSet.push_back(TempVect);
    		TempVect.resize(0);
    	}
    }
    //TrainingDataSet.push_back(TempVect);
    fin.close();
    return true;
}

void Normailze()
{
	vector<double> MaxVector;
	for(unsigned i = 1; i < TrainingDataSet[0].size(); i++)
	{
		double Max = 0;
		for(unsigned j = 0; j < TrainingDataSet.size(); j++)
		{
			if(Max < TrainingDataSet[j][i])
			{
				Max = TrainingDataSet[j][i];
			}
		}
		MaxVector.push_back(Max);
	}
	
	for(unsigned i = 1; i < TrainingDataSet[0].size(); i++)
	{
		
		for(unsigned j = 0; j < TrainingDataSet.size(); j++)
		{
			 TrainingDataSet[j][i] = TrainingDataSet[j][i] / MaxVector[i-1];
		}
		
	}
}

//return class lable of nearest instance
//if fail return 3 --> not a class
int NearestNeighbor( vector<int> ClassIndexUsingForNearestNeighbor, vector< vector < double > >  TrainingDataSetNN, vector<double> NewInstance) 
{
    int MinDpoint = TrainingDataSetNN.size();
	if(!NewInstance.empty())
	{
		
		float Min = 100000;
		
		//cycles through all points in training set and find closest
		for(unsigned j = 0; j < TrainingDataSetNN.size(); j++)
		{
		   float Temp = 0;
		   //cycle through all inexoffeatures we are using for eucledian distance
		   for(unsigned i = 0; i < ClassIndexUsingForNearestNeighbor.size(); i++)
		   {
			   //does eucledian distance per point per 
		       //Temp = Temp + pow((TrainingDataSetNN[j][ClassIndexUsingForNearestNeighbor[i]] - NewInstance[ClassIndexUsingForNearestNeighbor[i]]), 2);   
		       float Power = TrainingDataSetNN[j][ClassIndexUsingForNearestNeighbor[i]] - NewInstance[ClassIndexUsingForNearestNeighbor[i]];
		       Temp = Temp + (Power * Power);
		   	
		   }
		   Temp = sqrt(Temp);
		   //finds min distance
		   if(Min > Temp)
		   {
		      MinDpoint = j;
		      Min = Temp;
		   }
		   
	   }
	   return TrainingDataSetNN[MinDpoint][0];
	}
	return 3;
}


//Leave-one-out-validatior returns accuracy score
float Leave_one_out_validatior(vector<int> indexOfFeatures)
{
	float correct = 0.0;
	
	for(unsigned i = 0; i < TrainingDataSet.size(); i++)
	{
		vector<double> Temp_1 = TrainingDataSet[i];
		vector<vector<double> > Temp_2 = TrainingDataSet;
		Temp_2.erase(Temp_2.begin() + i);
		double ClassOfTemp1 = NearestNeighbor(indexOfFeatures, Temp_2, Temp_1);
		
		if(ClassOfTemp1 == Temp_1[0])
		{
			correct = correct + 1.0;
		}
	}
	
	return correct/ TrainingDataSet.size();
}

//SearchAlgorithms

//1) Forward
//start from initial state (empty set)
//add more features to previous set and evaluates the resulting feature subsets using the validator
//choose higet accuracy option 
//continue until no more features can be added/removed 
//report the feature subset with highest accuracy as best subset
void BestSubSetForward()
{
	vector <int> CurrentSetOfIndex;
	float CurrentSetPercentage = 0;
	vector<int> nLengthBestSetOfIndex;
	float nLengthBestPercentage = 0;
	vector <int> BestSetOfIndex;
	float BestSetPercentage = 0;
	
	for(unsigned i = 1; i <= TrainingDataSet[0].size(); i++)
	{
		nLengthBestPercentage = 0;
		CurrentSetOfIndex.resize(i);
		for(unsigned j = 1; j < TrainingDataSet[0].size(); j++)
		{
			if(find(nLengthBestSetOfIndex.begin(), nLengthBestSetOfIndex.end(), j) == nLengthBestSetOfIndex.end())
			{
				CurrentSetOfIndex[CurrentSetOfIndex.size()-1] = j; 
				CurrentSetPercentage = Leave_one_out_validatior(CurrentSetOfIndex);
				
				print(CurrentSetOfIndex, CurrentSetPercentage);
				if(CurrentSetPercentage > nLengthBestPercentage )
				{
					nLengthBestSetOfIndex = CurrentSetOfIndex;
					nLengthBestPercentage = CurrentSetPercentage;
				}
				
				if(CurrentSetPercentage > BestSetPercentage)
				{
					BestSetPercentage = CurrentSetPercentage;
					BestSetOfIndex = CurrentSetOfIndex;
				}
			}
		}
		CurrentSetOfIndex = nLengthBestSetOfIndex;
		
	}
	
	
	printFinal(BestSetOfIndex, BestSetPercentage );
}


//2) Backward
//start from initial state (FULL SET)
//add more features to previous set and evaluates the resulting feature subsets using the validator
//choose higet accuracy option 
//continue until no more features can be added/removed 
//report the feature subset with highest accuracy as best subset
void BestSubSetBackward()
{
	vector <int> CurrentSetOfIndex;
	float CurrentSetPercentage = 0;
	vector<int> nLengthBestSetOfIndex;
	float nLengthBestPercentage = 0;
	vector <int> BestSetOfIndex;
	float BestSetPercentage = 0;
	vector <int> SET;
	
	for(unsigned i = 1; i < TrainingDataSet[0].size(); i++ )
	{
		CurrentSetOfIndex.push_back(i);
	}
	
	//base case 
	CurrentSetPercentage = Leave_one_out_validatior(CurrentSetOfIndex);
	BestSetPercentage = CurrentSetPercentage;
	BestSetOfIndex = CurrentSetOfIndex;
	print(CurrentSetOfIndex, CurrentSetPercentage);
	SET = CurrentSetOfIndex;
	CurrentSetOfIndex.pop_back();
	//YOU HAVE FULL SET
	bool FIRST = true;
	while(!CurrentSetOfIndex.empty() )
	{
		nLengthBestPercentage = 0;
		
		if(FIRST)
		{
			FIRST = false;
		}
		else
		{
			CurrentSetPercentage = Leave_one_out_validatior(CurrentSetOfIndex);
			print(CurrentSetOfIndex, CurrentSetPercentage);
			if(CurrentSetPercentage > nLengthBestPercentage )
			{
				nLengthBestSetOfIndex = CurrentSetOfIndex;
				nLengthBestPercentage = CurrentSetPercentage;
			}
			
			if(CurrentSetPercentage > BestSetPercentage)
			{
				BestSetPercentage = CurrentSetPercentage;
				BestSetOfIndex = CurrentSetOfIndex;
			}
		}

		for(unsigned j = SET.size(); j > 1; j--)
		{
			if(j == SET.size())
			{
				CurrentSetOfIndex[SET.size() - j] = SET[j-1];
			}
			else
			{
				CurrentSetOfIndex[SET.size() - j] = SET[SET.size() - j-1];
			}
			
			CurrentSetPercentage = Leave_one_out_validatior(CurrentSetOfIndex);
			print(CurrentSetOfIndex, CurrentSetPercentage);	
			if(CurrentSetPercentage > nLengthBestPercentage )
			{
				nLengthBestSetOfIndex = CurrentSetOfIndex;
				nLengthBestPercentage = CurrentSetPercentage;
			}
			
			if(CurrentSetPercentage > BestSetPercentage)
			{
				BestSetPercentage = CurrentSetPercentage;
				BestSetOfIndex = CurrentSetOfIndex;
			}
			
		}
		CurrentSetOfIndex = nLengthBestSetOfIndex;
		SET = CurrentSetOfIndex;
		CurrentSetOfIndex.pop_back();
		
	}
	
	printFinal(BestSetOfIndex, BestSetPercentage );
}


int main()
{
	
	
	cout << "Enter name of File to get Data: \n";
	string NameOfFile;
	cin >> NameOfFile;
	Read(NameOfFile);
	Normailze();
	
	// cout << TrainingDataSet.size()  << ' ' << TrainingDataSet[0].size()<< endl;
	// vector<int> TestIndex = {1};
	// vector<vector<double>> TrainingDataSetNN = TrainingDataSet;
	// vector<double> NewInstance = TrainingDataSetNN[TrainingDataSetNN.size()-1];
	// TrainingDataSetNN.resize(TrainingDataSetNN.size()-1);
	// cout << NearestNeighbor(TestIndex, TrainingDataSetNN, NewInstance) << endl;
	
	// cout << Leave_one_out_validatior(TestIndex) << endl;
	//cout << TrainingDataSet[0][1] << endl <<TrainingDataSet[1][1];
	
	//printInitial();
	bool RUN = true;
	while(RUN)
	{
		int AlgorithmChoice = 0;
		while(AlgorithmChoice != 1 && AlgorithmChoice != 2 )
		{
		
			cout << "Type the algorithm you want to run: \n1)Forward Selection\n2)Backward Selection\n";
			cin >> AlgorithmChoice;
		}
		
		if(AlgorithmChoice == 1)
		{
		 	BestSubSetForward();
		}
		else if(AlgorithmChoice == 2)
		{
			BestSubSetBackward();
		}
		AlgorithmChoice = 0;
		while(AlgorithmChoice != 1 && AlgorithmChoice != 2 )
		{
		
			cout << "Run Again: \n1)Yes\n2)No\n";
			cin >> AlgorithmChoice;
		}
		if(AlgorithmChoice == 2)
		{
			RUN = false;
		}
		
	}
	
	
	return 0;
}