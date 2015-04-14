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
/*
double plane::findNormalPoint(point P){
	double result = normal.back();
	for (unsigned int i = 0; i < P.size(); i++){
		result += normal.at(i)*P.at(i);
	}
	return result;
}
*/

double plane::how_far_from_me(vector <double> point){ // (-d - Sum(Pi*Ci,from i = 0 to i = n-1))/Cn
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
