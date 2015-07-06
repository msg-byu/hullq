#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "plane.h"
using namespace std;

class hull
{
public:
	hull();
	void addPlane(plane);
	double distanceToHull(point);
	void buildHull(string);
	double findDistance(vector <double>);
	~hull();
private:
	vector <plane> planes;
};

