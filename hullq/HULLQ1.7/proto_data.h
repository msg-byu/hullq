#pragma once
#include "proto.h"
#include "simplex.h"
#include "hull.h"
#include <fstream>
#include <iostream>

using namespace std;

class proto_data
{
public:
	proto_data(string);
	int createProtos(string);
	string createQhullInput();
	void simplex_transform();
	void find_dist_to_hull();
	void find_vertex_to_hull();
	void vertexCheck(string);
	vector <vector <double> > dataout();
	vector <proto> GetProtos();
	string vertexOut();
	string firstPlane(int);
	string removeTrailingZeros(string);
	~proto_data();
private:
	vector <proto> database;
	hull myHull;
	string VDoutput;
	vector <vector <double> > data_conc_at;
	vector <vector <double> > vertexPoints;
};

