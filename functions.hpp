#pragma once

#include "structs.hpp"
#include <cmath>

double ComputeLight(Vec3 P, Vec3 N, Vec3 V, double s);

Vec3 ReflectRay(Vec3 R, Vec3 N);


Vec3 CanvasToViewport(int x, int y);


Color TraceRay(const Vec3& O, const Vec3& D, double t_min, double t_max, int recursion_depth);


std::pair<double, double> IntersectRaySphere(const Vec3& O, const Vec3& D, const Sphere& s);

std::pair<const Sphere*, double> ClosestIntersection(Vec3 O, Vec3 D, double t_min, double t_max);


constexpr unsigned int Cw = 600;
constexpr unsigned int Ch = 600;

constexpr double Vw = 2;
constexpr double Vh = 2;
constexpr double d = 1;

inline const Color BACKGROUND = { 255, 255, 255 };
inline const double INF = std::numeric_limits<double>::infinity();

