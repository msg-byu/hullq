#include "hull.h"

hull::hull()
{
}

void hull::addPlane(plane p){
	planes.push_back(p);
}


double hull::findDistance(vector <double> point){
	// finds the smallest distance from the point to the hull in the enthalpy direction then returns.
	double distance = 10000000; //needs to be a big number for initialization.
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
  // Builds the convexhull from the normals.in
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
	for (unsigned int i = 2+n; i < rawPoints.size(); i += n){ 
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
