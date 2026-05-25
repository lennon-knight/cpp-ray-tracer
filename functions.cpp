#include "functions.hpp"
//Globals

const std::vector<Light> lights = {
	{LightType::Ambient,     0.2, {},        {}        },
	{LightType::Point,       0.6, {2, 1, 0}, {}        },
	{LightType::Directional, 0.2, {},        {1, 4, 4},},
};

const std::vector<Sphere> scene = {
	{{ 0, -1, 3}, 1, {255, 0, 134},500,0.1},
	{{ 2,  0, 4}, 1, {134, 211, 255},5000,0.5},
	{{-2,  0, 4}, 1, {0, 255, 0},-1,0.2},
	{{0,-5001,0},5000,{255,255,0},1000,0.1}
};

//Calculate light intensity for each pixel a ray returns
double ComputeLight(Vec3 P, Vec3 N, Vec3 V, double s) {
	double i = 0.0;
	for (const auto& light : lights) {
		if (light.type == LightType::Ambient) {
			i += light.intensity;
		}
		else {
			Vec3 L;
			double t_max;
			if (light.type == LightType::Point) {
				L = light.position - P;
				t_max = 1.0;
			}
			else {
				L = light.direction;
				t_max = INF;
			}

			// Shadow check
			auto [shadow_sphere, shadow_t] = ClosestIntersection(P, L, 0.001, t_max);
			if (shadow_sphere != nullptr) {
				continue;
			}

			// Diffuse
			double n_dot_l = N.dot(L);
			if (n_dot_l > 0) {
				i += light.intensity * n_dot_l / (N.length() * L.length());
			}

			// Specular
			if (s != -1) {
				Vec3 R = N * (2 * N.dot(L)) - L;
				double r_dot_v = R.dot(V);
				if (r_dot_v > 0) {
					i += light.intensity * std::pow(r_dot_v / (R.length() * V.length()), s);
				}
			}
		}
	}
	return i;
}

Vec3 ReflectRay(Vec3 R, Vec3 N) {
	return N * (2 * N.dot(R)) - R;
}

//Translate canvas coordinates to viewport coordinates. (Canvas is the entire scene, viewport is a "window that determines the part of the scene the camera can "see")
Vec3 CanvasToViewport(int x, int y) {
	return Vec3(x * Vw / static_cast<double>(Cw),
		y * Vh / static_cast<double>(Ch),
		d);
}

//Return the two parametric variables on intersect that are used to track when/where a ray intersects a sphere if it does.
std::pair<double, double> IntersectRaySphere(const Vec3& O, const Vec3& D, const Sphere& s) {
	Vec3 CO = O - s.center;
	double a = D.dot(D);
	double b = 2.0 * CO.dot(D);
	double c = CO.dot(CO) - (s.radius * s.radius);

	double discriminant = b * b - 4 * a * c;

	if (discriminant < 0)
		return{ INF, INF };

	double sq = std::sqrt(discriminant);

	return{ (-b + sq) / (2 * a), (-b - sq) / (2 * a) };
}

std::pair<const Sphere*, double> ClosestIntersection(Vec3 O, Vec3 D, double t_min, double t_max) {
	const  Sphere* closest_sphere = nullptr;
	double closest_t = INF;

	for (const auto& sphere : scene) {
		auto [t1, t2] = IntersectRaySphere(O, D, sphere);
		if (t1 >= t_min && t1 <= t_max && t1 < closest_t) {
			closest_t = t1;
			closest_sphere = &sphere;
		}
		if (t2 >= t_min && t2 <= t_max && t2 < closest_t) {
			closest_t = t2;
			closest_sphere = &sphere;
		}
	}
	return { closest_sphere, closest_t };
}

//Trace the ray and return the pixel with intensity added.
Color TraceRay(const Vec3& O, const Vec3& D, double t_min, double t_max, int recursion_depth) {
	auto [closest_sphere, closest_t] = ClosestIntersection(O, D, t_min, t_max);
	if (closest_sphere == nullptr) {
		return BACKGROUND;
	}
	Vec3 P = D * closest_t + O;
	Vec3 N = P - closest_sphere->center;
	N = N / N.length();
	Vec3 V = D * -1;
	Color local_color = closest_sphere->color * ComputeLight(P, N, V, closest_sphere->specular);
	double r = closest_sphere->reflective;
	if (recursion_depth <= 0 || r <= 0) {
		return local_color;
	}

	Vec3 R = ReflectRay(V, N);
	Color reflected_color = TraceRay(P, R, 0.001, INF, recursion_depth - 1);

	return local_color * (1 - r) + reflected_color * r;
}