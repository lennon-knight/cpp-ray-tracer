//Basic Ray Tracer Using SFML
//Personal project following Computer Graphics from Scratch by Gabriel Gambetta
//Lennon Knight USF Senior in computer engineering.
//Much of my course work has been hardware or general programming in python, c, cpp usually with no libraries outside of cstd.
//This project is the beginning of my journey to bridge my programming knowledge and familiarity with hardware alongside my interest in computer graphics to build a competitive portfolio to break
//into the computer graphics space.
//This project is a basic ray tracer that utilizes the SFML graphics library. The project uses basic graphics computing strategies through simple matrix math, parametric representation, and some trig and algebra.

/*
 
Functionalities: 
Perspective projection through a virtual viewport
Ray - sphere intersection via the quadratic formula
A scene system with multiple objects
Three different light types(ambient, point, directional)
Diffuse(Lambertian) shading
Specular(Phong) highlights with material - dependent shininess
Shadow rays
Recursive mirror reflections with material - dependent reflectivity

*/

#include<SFML/Graphics.hpp>
#include "functions.hpp"

int main() {

	//Make a window
	sf::RenderWindow window(sf::VideoMode({ Cw, Ch }), "Ray Tracer");
	sf::Image image({ Cw, Ch }, sf::Color::Black);

	//Camera position vector
	Vec3 O(0, 0, 0);

	int recursion_depth = 3;

	//Center the coordinate system making 0,0,0 the center
	int half_w = static_cast<int>(Cw) / 2;
	int half_h = static_cast<int>(Ch) / 2;

	//Iterate through each pixel
	for (int y = half_h - 1; y >= -half_h; --y) {
		for (int x = -half_w; x < half_w; ++x) {

			//Convert pixel coordinate into a 3D direction D
			Vec3 D = CanvasToViewport(x, y);
			//Cast a ray from the camera, O, in the direction D, checking for intersections from t = 1 to infity: This is 99 percent of compute time.
			Color c = TraceRay(O, D, 1, INF, recursion_depth);

			//Write the pixel to the window
			unsigned int px = static_cast<unsigned int>(x + half_w);
			unsigned int py = static_cast<unsigned int>(half_h - 1 - y);
			image.setPixel({ px, py }, sf::Color(c.r, c.g, c.b));
		}
	}

	sf::Texture texture(image);
	sf::Sprite sprite(texture);

	while (window.isOpen()) {
		while (const auto event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}

		window.clear();
		window.draw(sprite);
		window.display();
	}
}