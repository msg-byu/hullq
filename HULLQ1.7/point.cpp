#include "point.h"


point::point(double X, double Y, double Z)
{
	x = X;
	y = Y;
	z = Z;
}

double point::getX(){
	return x;
}

double point::getY(){
	return y;
}

double point::getZ(){
	return z;
}

void point::setX(double X){
	x = X;
}

void point::setY(double Y){
	y = Y;
}

void point::setZ(double Z){
	z = Z;
}

point::~point()
{
}
