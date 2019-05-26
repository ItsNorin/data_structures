#pragma once
#include <functional>

// a point for a 2d graph
struct Point2D {
	int x, y;

	Point2D(int x = 0, int y = 0);

	double directDistance(const Point2D &n) const;

	bool operator==(const Point2D &n) const;
	bool operator!=(const Point2D &n) const;
};

// a point for a 3d graph
struct Point3D {
	int x, y, z;

	Point3D(int x = 0, int y = 0, int z = 0);

	double directDistance(const Point3D &n) const;

	bool operator==(const Point3D &n) const;
	bool operator!=(const Point3D &n) const;
};


// allows points to be hashed
namespace std {
	template<> 
	struct hash<Point2D>	{
		// generates a "unique" index using cantor pairing
		std::size_t operator()(const Point2D &s) const noexcept {
			int xPlusY = s.x + s.y;
			return s.y + ((xPlusY) * (xPlusY + 1)) / 2 ;
		}
	};

	template<> 
	struct hash<Point3D> {
		// its just cantor pairing twice.
		std::size_t operator()(const Point3D &s) const noexcept {
			int xPlusY = s.x + s.y;
			int zPlusXPairedY = s.y + ((xPlusY) * (xPlusY + 1)) / 2 + s.z;
			return s.z + ((zPlusXPairedY) * (zPlusXPairedY + 1)) / 2;
		}
	};
};