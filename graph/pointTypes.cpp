#include "pointTypes.h"
#include <cmath>


Point2D::Point2D(int x, int y) 
	: x(x), y(y)
{}

double Point2D::directDistance(const Point2D & n) const {
	int dX = x - n.x,
		dY = y - n.y;
	return sqrt((dX * dX) + (dY * dY));
}

bool Point2D::operator==(const Point2D & n) const { return (x == n.x) && (y == n.y); }
bool Point2D::operator!=(const Point2D & n) const { return !operator==(n); }



Point3D::Point3D(int x, int y, int z)
	: x(x), y(y), z(z)
{}

double Point3D::directDistance(const Point3D & n) const {
	int dX = x - n.x,
		dY = y - n.y,
		dZ = z - n.z;
	return sqrt((dX * dX) + (dY * dY) + (dZ * dZ));
}

bool Point3D::operator==(const Point3D & n) const { return (x == n.x) && (y == n.y) && (z == n.z); }
bool Point3D::operator!=(const Point3D & n) const { return !operator==(n); }