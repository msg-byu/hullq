#include "hull.h"

hull::hull()
{
}

void hull::addPlane(plane p){
	planes.push_back(p);
}
/*
double hull::distanceToHull(point p){
	//This function calculates distance in the z direction to the convex hull
	double dist,zHull;
	dist = DBL_MAX; //I dont like this, but hopefully this is larger than the largest distance we will ever see. 
	for (auto plane : planes){
		zHull = plane.findZ(p.getX(),p.getY());
		if (p.getZ() - zHull <= dist){
			dist = p.getZ() - zHull;
		}
	}
	return dist;
}
*/

double hull::findDistance(vector <double> point){
	// finds the smallest distance from the point to the hull in the enthralpic direction then returns.
	double distance = 10000000; //needs to be a big number
	double d = 0;
	for (auto p : planes){
		d = p.how_far_from_me(point);
		//cout << "   Distance: " << d << "\n";
		if (d < distance){
			distance = d;
		}
	}
	return distance;
}

void hull::buildHull(string hullFile){
	ifstream myFile;
	myFile.open(hullFile);
	string token, line;
	stringstream temp;
	vector <double> rawPoints;

	while (getline(myFile, line, '\n')){
		//line = line.substr(line.find_first_not_of(" "), line.size());
		temp << line;
		while (getline(temp, token, ' ')){
			if (!token.empty()){
				rawPoints.push_back(stod(token));
			}
		}
		temp.clear();
	}

	int n = rawPoints[0];
	plane myPlane = plane();
	for (unsigned int i = 2+n; i < rawPoints.size(); i += n){ // we used 5 because this will remove the first plane assuming that this is the plane that caps the convex hull.
		for (int j = 0; j < n; j++){
			myPlane.addCoefficent(rawPoints.at(i+j));
		}
		addPlane(myPlane); 
		myPlane.clear();
	}
	myFile.close();
}

hull::~hull()
{
}
