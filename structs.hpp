#pragma once
#include <cmath>
#include <vector>
#include <limits>
#include <algorithm>

struct Vec3 {
	double x, y, z;
	Vec3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

	Vec3 operator  + (const Vec3& v) const { return { x + v.x, y + v.y, z + v.z }; }
	Vec3 operator - (const Vec3& v) const { return { x - v.x, y - v.y, z - v.z }; }
	double dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
	double length() const { return std::sqrt(x * x + y * y + z * z); }
	Vec3 operator  * (const double& v) const { return { x * v, y * v, z * v }; }
	Vec3 operator  / (const double& v) const { return { x / v, y / v, z / v }; }
};

struct Color {
	int r, g, b;
	Color operator*(double v) const {
		return { std::clamp(static_cast<int>(r * v), 0, 255),
				 std::clamp(static_cast<int>(g * v), 0, 255),
				 std::clamp(static_cast<int>(b * v), 0, 255) };
	}
	Color operator+(const Color& c) const {
		return { std::clamp(r + c.r, 0, 255),
				 std::clamp(g + c.g, 0, 255),
				 std::clamp(b + c.b, 0, 255) };
	}
};

struct Sphere {
	Vec3 center;
	double radius;
	Color color;
	double specular;
	double reflective;
};

//Light Types
enum class LightType {
	Ambient,
	Point,
	Directional
};

struct Light {
	LightType type;
	double intensity;
	Vec3 position;
	Vec3 direction;
};