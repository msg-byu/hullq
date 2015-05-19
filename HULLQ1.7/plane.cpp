#include "plane.h"


plane::plane()
{

}

void plane::clear(){
	normal.clear();
}

void plane::setNormal(vector <double> newNormal){
	normal = newNormal;
}

void plane::addCoefficent(double c){
	normal.push_back(c);
}


double plane::how_far_from_me(vector <double> point){ 
  // This is the logic behind calculating distance. (-d - Sum(Pi*Ci,from i = 0 to i = n-1))/Cn
  // We used normals.in file for plane equations. 
	double result = -normal.back();
	vector <double> myPoint = point;
	unsigned int c = 0;
	for (; c < normal.size() - 2; c++){
		result = result-normal.at(c)*myPoint.at(c);
		//cout << myPoint.at(c) << " ";
	}
	//cout << -normal.back() <<"\n";
	result = result/normal.at(c);
	//cout << abs(result) << "\n";
	//       	if (abs(myPoint.back() - result) < .000001){
	//	return 0;
      		//	}
	return abs(myPoint.back() - result);
}

plane::~plane()
{
}
