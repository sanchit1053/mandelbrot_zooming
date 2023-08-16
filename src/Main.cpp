/*
	MANDELBROT ZOOMING
				-Sanchit Jindal

Made and tested on Windows

Using the SFML package and the SFML boiler plate
*/

#include "Platform/Platform.hpp"
#include <bits/stdc++.h>

using namespace std;
using namespace sf;

int type = 3;
/*
There are different type of algorithm used to produce the mandelbrot set

0: THe Naive approach
	We move through each pixel of the screen and calculate the mandelbrot escape time to
	color the pixel

1: Using quadTrees
	Using the concept of quad trees we can significantly reduce the number of pixels needed to be checked
	only some pixels of the box are checked so sometimes in zooming there is dithering and loss of quality

2: quadtrees 2
	A better approach to the quad trees
	We check the whole boundary of the box instead of only some key factors
	This makes a much cleaner image but requires a little more time

3: showing the quad trees
	in this type we can show the whole boxes of the quad tree which are filled
	It is a good representation of how the quad trees can reduce the computations

4: printing on terminal
	Making a mandelbrot set on the terminal
*/

int height = 512;
int width = 512;
// The height and width of the screen formed keep them at power of 2 for best result

int MAX_ITER = 100;
// max number of times iterating for each pixel
// increase for higher resolution but more time


double real1 = 0;
double real2 = 0;
double imag1 = 0;
double imag2 = 0;
/*
Global Variables for the boundary of the mandelbrot set showing
	real1 is lower value in the real number line
	real2 is higher value in the real number line
	imag1 is lower value in the real number line
	imag2 is higher value in the real number line
*/


double centerx = -0.8;
double centery = 0;
// Global variables to keep the value of the point on the plane where we are zooming

double centerx1 = -1.04082816210546;
double centery1 = 0.346341718848392;
double centerx2 = -0.761574;
double centery2 = -0.0847596;
double centerx3 = -1.5;
double centery3 = -0;
// some predefined points to zoom onto

double boxlength = 0.1;
// A parameter to tell how close to a predefined point we need to click to zoom to it


double screen = 1.25;
// A scaling factor DONT CHANGE

double initfactor = 1.5;

int man(double real, double imag);
// A function to return the escape time of the pixel (real,imag)

bool fill(double x1, double x2, double y1, double y2, double x, double y, double w, double h, RenderWindow &window);
/*
A function to check if the box(x1,y1,x2,y2) has the same color
and fill it if it does
Type 1;
checks the values of corners, center and all the midpoints of edges
*/
void mandelbrot(double real1, double real2, double imag1, double imag2, double x, double y, double w, double h, RenderWindow &window);
// Function called to produce the mandelbrot image with type 1
// for between real1, imag1 to real2,imag2 in the box x,y with width w and height h

bool fill2(double x1, double x2, double y1, double y2, double x, double y, double w, double h, RenderWindow &window);
/*
A function to check if the box(x1,y1,x2,y2) has the same color
and fill it if it does
Type 2;
checks the values of all boundaryies and the center pixels to get a better estimate
*/

void mandelbrot2(double real1, double real2, double imag1, double imag2, double x, double y, double w, double h, RenderWindow &window);
// Function called to produce the mandelbrot image with type 2
// for between real1, imag1 to real2,imag2 in the box x,y with width w and height h

bool fill3(double x1, double x2, double y1, double y2, double x, double y, double w, double h, RenderWindow &window);
/*
A function to check if the box(x1,y1,x2,y2) has the same color
and fill it if it does
Type 3
produces a border to the box to show the implementation of quad trees
*/
void mandelbrot3(double real1, double real2, double imag1, double imag2, double x, double y, double w, double h, RenderWindow &window);
// Function called to produce the mandelbrot image with type 3
// for between real1, imag1 to real2,imag2 in the box x,y with width w and height h

Color hsvcolor(int hue, float sat, float val);
// A function to return a color given hue, saturation and value
// as sfml does not have a hsv color scheme a custom funtion

Color col(int i);
// function to return the color of the pixel if escape time is i

/*
=========================================================================================================
				MAIN FUNCTION
=========================================================================================================
*/

int main()
{

	if (type == 4)
	{
		height = 40;
		width = 150;

		real1 = -1.5;
		real2 = 1.5;
		imag1 = -1.5;
		imag2 = 1.5;
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				int m = man(i, j);
				if (m >= MAX_ITER)
				{
					cout << "@";
				}
				else
				{
					cout << " ";
				}
			}
			cout << endl;
		}
		return 0;
	}

	util::Platform platform;

	// #if defined(_DEBUG)
	// 	std::cout << "Hello World!" << std::endl;
	// #endif

	RenderWindow window;
	// in Windows at least, this must be called before creating the window
	double screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());

	// Use the screenScalingFactor
	// producing the screen without restrictionn that it cannot be resized
	window.create(VideoMode(height * screenScalingFactor, width * screenScalingFactor), "Mandelbrot", Style::Titlebar | Style::Close);
	platform.setIcon(window.getSystemHandle());

	// CircleShape shape(1.f * screenScalingFactor);
	// shape.setFillColor(Color::Red);

	double factor = initfactor;
	// a factor to decrease in each loop to zoom into the figure

	real1 = centerx - factor;
	real2 = centerx + factor;
	imag1 = centery - factor;
	imag2 = centery + factor;
	// the value of corrdinates on the center provided and using the factor


	if (type == 0)
	{
	}
	else if (type == 3)
	{

		mandelbrot3(real1, real2, imag1, imag2, 0, 0, width, height, window);
		// making a mandelbrot set as the home page
	}
	else
	{
		mandelbrot2(real1, real2, imag1, imag2, 0, 0, width, height, window);
		// making a mandelbrot set as the home page
	}
	if (type != 0)
	{
		double circlecenterx;
		double circlecentery;
		double radius = 10;

		CircleShape shape1(radius * screenScalingFactor);
		circlecenterx = (centerx1 - real1) / (real2 - real1) * width;
		circlecentery = (centery1 - imag1) / (imag2 - imag1) * height;
		shape1.setPosition((circlecenterx - radius) * screenScalingFactor, (circlecentery - radius) * screenScalingFactor);
		shape1.setFillColor(Color::Transparent);
		shape1.setOutlineThickness(1);
		window.draw(shape1);

		CircleShape shape2(radius * screenScalingFactor);
		circlecenterx = (centerx2 - real1) / (real2 - real1) * width;
		circlecentery = (centery2 - imag1) / (imag2 - imag1) * height;
		shape2.setPosition((circlecenterx - radius) * screenScalingFactor, (circlecentery - radius) * screenScalingFactor);
		shape2.setFillColor(Color::Transparent);
		shape2.setOutlineThickness(1);
		window.draw(shape2);

		CircleShape shape3(radius * screenScalingFactor);
		circlecenterx = (centerx3 - real1) / (real2 - real1) * width;
		circlecentery = (centery3 - imag1) / (imag2 - imag1) * height;
		shape3.setPosition((circlecenterx - radius) * screenScalingFactor, (circlecentery - radius) * screenScalingFactor);
		shape3.setFillColor(Color::Transparent);
		shape3.setOutlineThickness(1);
		window.draw(shape3);
	}
	window.display();

	Event event;
	// defining a event to poll

	bool running = false;
	// variable to check if zooming is going on

	while (window.isOpen())
	// main loop of the program
	{
		while (window.pollEvent(event))
		// polling events so as to prevent crashing
		{
			if (event.type == Event::Closed)
			// closing the window
			{
				window.close();
			}

			if (event.type == Event::MouseButtonPressed)
			// selecting the point to which to point to
			{
				if (event.mouseButton.button == Mouse::Left)
				{
					double mousex = real1 + event.mouseButton.x / screenScalingFactor / width * (real2 - real1);
					double mousey = imag1 + event.mouseButton.y / screenScalingFactor / height * (imag2 - imag1);
					if (abs(mousex - centerx1) <= boxlength && abs(mousey - centery1) <= boxlength && running == false)
					// if mouse is clicked around point 1 at a distance between boxlength
					{
						centerx = centerx1;
						centery = centery1;
						// cout << "number 1" << endl;
						running = true;
					}

					else if (abs(mousex - centerx2) <= boxlength && abs(mousey - centery2) <= boxlength && running == false)
					// if mouse is clicked around point 2 at a distance between boxlength
					{
						centerx = centerx2;
						centery = centery2;
						// cout << "number 2" << endl;
						running = true;
					}

					else if (abs(mousex - centerx3) <= boxlength && abs(mousey - centery3) <= boxlength && running == false)
					// if mouse is clicked around point 3 at a distance between boxlength
					{
						centerx = centerx3;
						centery = centery3;
						// cout << "number 3" << endl;
						running = true;
					}
				}
			}

			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Escape)
				// checking if escape is pressed to reset the graph
				{
					factor = initfactor;
					running = false;

					real1 = centerx - factor;
					real2 = centerx + factor;
					imag1 = centery - factor;
					imag2 = centery + factor;

					mandelbrot2(real1, real2, imag1, imag2, 0, 0, width, height, window);

					double circlecenterx;
					double circlecentery;
					double radius = 10;

					CircleShape shape1(radius * screenScalingFactor);
					circlecenterx = (centerx1 - real1) / (real2 - real1) * width;
					circlecentery = (centery1 - imag1) / (imag2 - imag1) * height;
					shape1.setPosition((circlecenterx - radius) * screenScalingFactor, (circlecentery - radius) * screenScalingFactor);
					shape1.setFillColor(Color::Transparent);
					shape1.setOutlineThickness(1);
					window.draw(shape1);

					CircleShape shape2(radius * screenScalingFactor);
					circlecenterx = (centerx2 - real1) / (real2 - real1) * width;
					circlecentery = (centery2 - imag1) / (imag2 - imag1) * height;
					shape2.setPosition((circlecenterx - radius) * screenScalingFactor, (circlecentery - radius) * screenScalingFactor);
					shape2.setFillColor(Color::Transparent);
					shape2.setOutlineThickness(1);
					window.draw(shape2);

					CircleShape shape3(radius * screenScalingFactor);
					circlecenterx = (centerx3 - real1) / (real2 - real1) * width;
					circlecentery = (centery3 - imag1) / (imag2 - imag1) * height;
					shape3.setPosition((circlecenterx - radius) * screenScalingFactor, (circlecentery - radius) * screenScalingFactor);
					shape3.setFillColor(Color::Transparent);
					shape3.setOutlineThickness(1);
					window.draw(shape3);
					window.display();
				}
			}
		}

		// double real1 = -2;
		// double real2 = 2;
		// double imag1 = -2;
		// double imag2 = 2;
		// cout << "hello";

		// mandelbrot(real1, real2, imag1, imag2, 0, 0, width, height, window);

		if (running)
		{
			if (type == 0)
			{
				for (int i = 0; i < height * screenScalingFactor; i++)
				// going through each pixel
				{
					for (int j = 0; j < width * screenScalingFactor; j++)
					{
						// needed as the time taken is vey high so
						// need a poll to proevint crashing
						while (window.pollEvent(event))
						{
							if (event.type == Event::Closed)
								window.close();
						}

						// double real = real1 + double(i) / width * (real2 - real1);
						// double imag = imag1 + double(j) / width * (imag2 - imag1);

						// scaling the value fo
						int m = man(i, j);

						// making a rectangle of one pixel at the point
						RectangleShape rect(Vector2f(1 * screenScalingFactor, 1 * screenScalingFactor));
						rect.setPosition(i * screenScalingFactor, j * screenScalingFactor);
						// Color color(col(m), 255 - 255 * m / MAX_ITER, 255 - 255 * m / MAX_ITER);
						rect.setFillColor(col(m));
						window.draw(rect);
						window.display();
					}
				}
			}

			if (type == 1)
			// for type 1
			{

				real1 = centerx - factor;
				real2 = centerx + factor;
				imag1 = centery - factor;
				imag2 = centery + factor;
				// calculating the bounds to be shown in the screen

				mandelbrot(real1, real2, imag1, imag2, 0, 0, width, height, window);
				//producing the mandelbrot set

				window.display();
				window.clear();

				factor = factor / 1.1;
				// decreasing the factor to be used in the next loop
			}

			else if (type == 2)
			// for type 2
			{
				real1 = centerx - factor;
				real2 = centerx + factor;
				imag1 = centery - factor;
				imag2 = centery + factor;
				// calculating the bounds to be shown in the screen

				mandelbrot2(real1, real2, imag1, imag2, 0, 0, width, height, window);
				// proding the mandelbrot set
				window.display();
				window.clear();

				factor = factor / 1.1;
				// decreasing the factor to be used in the next loop
			}

			else if (type == 3)
			// for type 3
			{
				real1 = centerx - factor;
				real2 = centerx + factor;
				imag1 = centery - factor;
				imag2 = centery + factor;
				// calculating the bounds to be shown in the screen

				mandelbrot3(real1, real2, imag1, imag2, 0, 0, width, height, window);
				// producing the mandelbrot set

				window.display();
				window.clear();

				factor = factor / 1.1;
				// decreasing the factor to be used in the next loop
			}
		}
		// window.clear();
		// window.draw(shape);
		// window.display();
	}
	return 0;
}

/*
======================================================================================================
							COLORS
======================================================================================================
*/

// color to return different color for all excapetime
Color col(int i)
{
	if (i >= MAX_ITER)
	{
		// if escape time is MAX_ITER then the pixel is inside the pattern and thus the color is black
		return Color(0, 0, 0);
	}
	else
	{
		// using the hsv color to produce a new color
		return hsvcolor(i * 360 / MAX_ITER, 100, 100);
	}
}

Color hsvcolor(int hue, float sat, float val)
{
	hue = hue % 360;
	// limiting the value of hue to less than 360

	if (sat < 0)
	{
		sat = 0;
	}
	if (sat > 1)
	{
		sat = 1;
	}
	// limiting the value of sat to between 0 and 1

	if (val < 0)
	{
		val = 0;
	}
	if (val > 1)
	{
		val = 1;
	}
	// limiting the value of val between 0 to 1

	// producing some variable to be used in the color value
	// using a form of code in https://en.sfml-dev.org/forums/index.php?topic=7313.0

	int h = hue / 60;
	float f = float(hue) / 60 - h;
	float p = val * (1 - sat);
	float q = val * (1 - sat * f);
	float t = val * (1 - sat * (1 - f));

	if (h == 0)
	{
		return Color(val * 255, t * 255, p * 255);
	}
	if (h == 1)
	{
		return Color(q * 255, val * 255, p * 255);
	}
	if (h == 2)
	{
		return Color(p * 255, val * 255, t * 255);
	}
	if (h == 3)
	{
		return Color(p * 255, q * 255, val * 255);
	}
	if (h == 4)
	{
		return Color(t * 255, p * 255, val * 255);
	}
	if (h == 5)
	{
		return Color(val * 255, p * 255, q * 255);
	}
	else
	{
		return Color(val * 255, t * 255, p * 255);
	}
}

/*
======================================================================================================
					TYPE 1
======================================================================================================
*/

// producing mandelbort function of type 1
void mandelbrot(double real1, double real2, double imag1, double imag2, double x, double y, double w, double h, RenderWindow &window)
{
	if (!fill(x, x + w - 1, y, y + h - 1, x, y, w, h, window))
	// if the part of the rectangle is of same color then fill
	// if not then rectangle is divided into 4 parts
	{
		mandelbrot(real1, real1 + (real2 - real1) / 2, imag1, imag1 + (imag2 - imag1) / 2, x, y, w / 2, h / 2, window);
		// the top left region
		mandelbrot(real1 + (real2 - real1) / 2, real2, imag1, imag1 + (imag2 - imag1) / 2, x + w / 2, y, w / 2, h / 2, window);
		// the top right region
		mandelbrot(real1, real1 + (real2 - real1) / 2, imag1 + (imag2 - imag1) / 2, imag2, x, y + h / 2, w / 2, h / 2, window);
		// the bottom left region
		mandelbrot(real1 + (real2 - real1) / 2, real1, imag1 + (imag2 - imag1) / 2, imag2, x + w / 2, y + h / 2, w / 2, h / 2, window);
		// the bottom right region
	}

	return;
}

// the rectange producer for type 1
bool fill(double x1, double x2, double y1, double y2, double x, double y, double w, double h, RenderWindow &window)
{
	if (w == 1 && h == 1)
	// if only one pixel color it by finding the escape number
	{

		int c = man(x1, y1);
		// the escape time

		RectangleShape rect(Vector2f(1 * screen, 1 * screen));
		// Color color(255 - 255 * c / MAX_ITER, 255 - 255 * c / MAX_ITER, 255 - 255 * c / MAX_ITER);
		// greyscale color
		rect.setFillColor(col(c));
		rect.setPosition(x * screen, y * screen);
		window.draw(rect);

		return true;
	}

	if (w == 2 && h == 2)
	// if two by two only check the four corners
	{
		int c = man(x1, y1);
		if (man(x2, y1) != c)
		{
			return false;
		}
		if (man(x1, y2) != c)
		{
			return false;
		}
		if (man(x2, y2) != c)
		{
			return false;
		}
		// if all not same then return false and dont fill

		RectangleShape rect(Vector2f(w * screen, h * screen));
		// Color color(255 - 255 * c / MAX_ITER, 255 - 255 * c / MAX_ITER, 255 - 255 * c / MAX_ITER);
		// grey scale color

		rect.setFillColor(col(c));
		rect.setPosition(x * screen, y * screen);
		window.draw(rect);
		// if all values same then make a square and return true
		return true;
	}

	//other wise if more than 2*2 then check corners mid point and mid point of edges

	int c = man(x1, y1);
	// center
	if (man((x1 + x2) / 2, (y1 + y2) / 2) != c)
	{
		return false;
	}

	// midpoint
	if (man((x1 + x2) / 2, y1) != c)
	{
		return false;
	}
	if (man((x1 + x2) / 2, y2) != c)
	{
		return false;
	}
	if (man(x1, (y1 + y2) / 2) != c)
	{
		return false;
	}
	if (man(x2, (y1 + y2) / 2) != c)
	{
		return false;
	}

	// remaing corners
	if (man(x2, y1) != c)
	{
		return false;
	}
	if (man(x1, y2) != c)
	{
		return false;
	}
	if (man(x2, y2) != c)
	{
		return false;
	}
	//if any time the escape time is not same then return false with out making rectangle

	// if (man((3 * x1 + x2) / 4, (y1 + y2) / 2) != c)
	// 	return false;
	// if (man((x1 + 3 * x2) / 4, (y1 + y2) / 2) != c)
	// 	return false;
	// if (man((x1 + x2) / 2, (3 * y1 + y2) / 4) != c)
	// 	return false;

	RectangleShape rect(Vector2f(w * screen, h * screen));
	// Color color(255 - 255 * c / MAX_ITER, 255 - 255 * c / MAX_ITER, 255 - 255 * c / MAX_ITER);
	// grey scale color

	rect.setFillColor(col(c));
	rect.setPosition(x * screen, y * screen);
	window.draw(rect);
	// make a rectangle and return true
	return true;
}

/*
=============================================================================================================
					TYPE 2
=============================================================================================================
*/

// making a mandelbrot of type 2
void mandelbrot2(double real1, double real2, double imag1, double imag2, double x, double y, double w, double h, RenderWindow &window)
{
	// if the part of the rectangle is of same color then fill
	// if not then rectangle is divided into 4 parts
	if (!fill2(x, x + w - 1, y, y + h - 1, x, y, w, h, window))
	{
		mandelbrot2(real1, real1 + (real2 - real1) / 2, imag1, imag1 + (imag2 - imag1) / 2, x, y, w / 2, h / 2, window);
		// top left
		mandelbrot2(real1 + (real2 - real1) / 2, real2, imag1, imag1 + (imag2 - imag1) / 2, x + w / 2, y, w / 2, h / 2, window);
		// top right
		mandelbrot2(real1, real1 + (real2 - real1) / 2, imag1 + (imag2 - imag1) / 2, imag2, x, y + h / 2, w / 2, h / 2, window);
		// bottom left
		mandelbrot2(real1 + (real2 - real1) / 2, real1, imag1 + (imag2 - imag1) / 2, imag2, x + w / 2, y + h / 2, w / 2, h / 2, window);
		// bottom right
	}
	return;
}

// rectangle producer of type 2
bool fill2(double x1, double x2, double y1, double y2, double x, double y, double w, double h, RenderWindow &window)
{
	// if only one pixel give the pixel color corrorsponding to its escape time
	if (w == 1 && h == 1)
	{
		int c = man(x1, y1);
		RectangleShape rect(Vector2f(1 * screen, 1 * screen));
		// Color color(255 - 255 * c / MAX_ITER, 255 - 255 * c / MAX_ITER, 255 - 255 * c / MAX_ITER);
		// grey scale color

		rect.setFillColor(col(c));
		rect.setPosition(x * screen, y * screen);
		window.draw(rect);
		return true;
	}

	// if 2*2 pixel then only check all the pixels for same escape time
	if (w == 2 && h == 2)
	{
		int c = man(x1, y1);
		if (man(x2, y1) != c)
		{
			return false;
		}
		if (man(x1, y2) != c)
		{
			return false;
		}
		if (man(x2, y2) != c)
		{
			return false;
		}
		// if different time then dont make a square and return false

		RectangleShape rect(Vector2f(w * screen, h * screen));

		// Color color(255 - 255 * c / MAX_ITER, 255 - 255 * c / MAX_ITER, 255 - 255 * c / MAX_ITER);
		// grey scale color

		rect.setFillColor(col(c));
		rect.setPosition(x * screen, y * screen);
		window.draw(rect);
		return true;
	}

	//bool same = true;
	int c = man(x1, y1);
	// escape time of the corner

	float xincrement = 1;
	float yincrement = 1;
	// variable to increase the boundary jumps

	if (man((x1 + x2) / 2, (y1 + y2) / 2) != c)
	{
		return false;
	}
	// check with escape time of the center of square

	for (int i = 0; i < int(w); i++)
	{
		if (man(x1 + i * xincrement, y1) != c)
		{
			return false;
		}
		// if escape time not same in top then retrun false

		if (man(x1 + i * xincrement, y2) != c)
		{
			return false;
		}
		// if escape time not same in bottom then retrun false
	}

	for (int j = 0; j < int(h); j++)
	{
		if (man(x1, y1 + j * yincrement) != c)
		{
			return false;
		}
		// if escape time not same in left then retrun false
		if (man(x2, y1 + j * yincrement) != c)
		{
			return false;
		}
		// if escape time not same in right then retrun false
	}

	RectangleShape rect(Vector2f(w * screen, h * screen));
	// Color color(255 - 255 * c / MAX_ITER, 255 - 255 * c / MAX_ITER, 255 - 255 * c / MAX_ITER);
	// grey scale color
	rect.setFillColor(col(c));
	rect.setPosition(x * screen, y * screen);
	window.draw(rect);
	// if all pixels same then dray the square and then return true
	return true;
}

/*
======================================================================================================================
						TYPE 3
======================================================================================================================
*/

// making a mandelbrot of type 3
void mandelbrot3(double real1, double real2, double imag1, double imag2, double x, double y, double w, double h, RenderWindow &window)
{
	// if the part of the rectangle is of same color then fill
	// if not then rectangle is divided into 4 parts
	if (!fill3(x, x + w - 1, y, y + h - 1, x, y, w, h, window))
	{
		mandelbrot3(real1, real1 + (real2 - real1) / 2, imag1, imag1 + (imag2 - imag1) / 2, x, y, w / 2, h / 2, window);
		// top left
		mandelbrot3(real1 + (real2 - real1) / 2, real2, imag1, imag1 + (imag2 - imag1) / 2, x + w / 2, y, w / 2, h / 2, window);
		// top right
		mandelbrot3(real1, real1 + (real2 - real1) / 2, imag1 + (imag2 - imag1) / 2, imag2, x, y + h / 2, w / 2, h / 2, window);
		// bottom left
		mandelbrot3(real1 + (real2 - real1) / 2, real1, imag1 + (imag2 - imag1) / 2, imag2, x + w / 2, y + h / 2, w / 2, h / 2, window);
		// bottom right
	}
	return;
}

// rectangle porducer of type 3
bool fill3(double x1, double x2, double y1, double y2, double x, double y, double w, double h, RenderWindow &window)
{
	// if only one pixel give it a color corresponding to the value of escape time
	if (w == 1 && h == 1)
	{
		int c = man(x1, y1);
		// escape time of the pixel
		RectangleShape rect(Vector2f(1 * screen, 1 * screen));
		// Color color(255 - 255 * c / MAX_ITER, 255 - 255 * c / MAX_ITER, 255 - 255 * c / MAX_ITER);
		// grey scale color

		rect.setFillColor(col(c));
		rect.setPosition(x * screen, y * screen);
		window.draw(rect);

		return true;
	}
	// if a 2 by 2 box then check all pixels
	if (w == 2 && h == 2)
	{
		int c = man(x1, y1);
		if (man(x2, y1) != c)
		{
			return false;
		}
		if (man(x1, y2) != c)
		{
			return false;
		}
		if (man(x2, y2) != c)
		{
			return false;
		}
		// if the pixel are not equal then return false and not make any rectangle

		RectangleShape rect(Vector2f(w * screen, h * screen));
		// Color color(255 - 255 * c / MAX_ITER, 255 - 255 * c / MAX_ITER, 255 - 255 * c / MAX_ITER);
		// grey color

		rect.setFillColor(col(c));
		rect.setPosition(x * screen, y * screen);
		window.draw(rect);
		return true;
	}

	//bool same = true;
	int c = man(x1, y1);
	// escape time of the corner

	float xincrement = 1;
	float yincrement = 1;
	// variable to increase the boundary jumps

	if (man((x1 + x2) / 2, (y1 + y2) / 2) != c)
	{
		return false;
	}
	// check with escape time of the center of square

	for (int i = 0; i < int(w); i++)
	{
		if (man(x1 + i * xincrement, y1) != c)
		{
			return false;
		}
		// if escape time not same in top then retrun false

		if (man(x1 + i * xincrement, y2) != c)
		{
			return false;
		}
		// if escape time not same in bottom then retrun false
	}

	for (int j = 0; j < int(h); j++)
	{
		if (man(x1, y1 + j * yincrement) != c)
		{
			return false;
		}
		// if escape time not same in left then retrun false
		if (man(x2, y1 + j * yincrement) != c)
		{
			return false;
		}
		// if escape time not same in right then retrun false
	}

	RectangleShape rect(Vector2f(w * screen, h * screen));
	// Color color(255 - 255 * c / MAX_ITER, 255 - 255 * c / MAX_ITER, 255 - 255 * c / MAX_ITER);
	// grey color scale

	rect.setFillColor(col(c));
	rect.setPosition(x * screen, y * screen);

	rect.setOutlineThickness(-1);
	// making a border for the quad tree
	window.draw(rect);
	return true;
}

/*
=====================================================================================================================
					ESCAPE TIME
=====================================================================================================================
*/

// return the escape time given the pixel coordinates
int man(double real, double imag)
{
	// scaling the coordinates to boundary
	real = real1 + float(real) / width * (real2 - real1);
	imag = imag1 + float(imag) / height * (imag2 - imag1);

	double tempx = 0;
	double tempy = 0;
	double zx = 0;
	double zy = 0;

	int iter = 0;
	while (zx + zy <= 16 && iter < MAX_ITER)
	{
		tempy = (tempx + tempx) * tempy + imag;
		tempx = zx - zy + real;
		zx = tempx * tempx;
		zy = tempy * tempy;
		iter++;
	}
	return iter;
}
