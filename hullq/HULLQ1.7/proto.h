#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <iostream>
using namespace std;

class proto
{
public:
	proto(string);
	vector <double> proto_val(); // returns last n-1 and enthalpy
	void setVals(vector <double>);
	void setDistance(double);
	string getProto();
	string getName();
	string outputVals(bool);
	double getDistance();
	~proto();
private:
	string prototype, name;
	double enthalpy, ent_temp, distance_to_hull;
	vector <double> concentration_coordinates;
	vector <double> orig_coor;
};

