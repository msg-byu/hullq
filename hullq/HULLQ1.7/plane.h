#pragma once
#include "point.h"
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class plane
{
public:
	plane();
	double findNormalPoint(point);
	void addCoefficent(double);
	void clear();
	void setNormal(vector<double>);
	double how_far_from_me(vector <double>);
	~plane();
private:
	vector <double> normal;
};

