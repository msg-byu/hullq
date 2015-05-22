#pragma once
#include <vector>
using namespace std;

class point
{
public:
	point(double, double, double);
	double getX();
	double getY();
	double getZ();
	void setX(double);
	void setY(double);
	void setZ(double);
	~point();
private:
	double x, y, z;
};

