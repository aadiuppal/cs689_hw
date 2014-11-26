/* object-oriented example C++ code */

#include <stdio.h>
#include <list>

/* this is an "abstract class" because at least one of its methods is not
 * defined 
 */
class Shape {
private:
	int my_serial_number;
	static int shape_counter;
public:

	// constructor: initialize this object

	Shape (void) {
		my_serial_number = shape_counter++;
		printf ("constructing shape #%d\n", my_serial_number);
	}

	// in C++ we must explicitly mark a method as virtual so it can be
	// overridden by methods inheriting from it

	virtual void draw (void) = 0;

	// a destructor: what to do when we are done with this class.
	// must also be virtual if we want things to work correctly

	virtual ~Shape (void) {
		printf ("destroying shape #%d\n", my_serial_number);
	}
};

/* this must be declared outside of the function definition
 * to be initialized properly 
 */

int Shape::shape_counter = 1;

/* a derived class, inheriting from Shape */

class Circle : public Shape {
private:
	int x, y, radius;

public:
	Circle (int _x, int _y, int _radius) : Shape () {
		x = _x;
		y = _y;
		radius = _radius;
	}

	virtual void draw (void) {
		printf ("draw a circle at x=%d, y=%d, radius=%d\n", x, y, radius);
	}

	~Circle (void) {
		printf ("destroying circle\n");
	}
};


class Line : public Shape {
private:
	int x0, y0, x1, y1;

public:
	Line (int _x0, int _y0, int _x1, int _y1) : Shape () {
		x0 = _x0;
		x1 = _x1;
		y0 = _y0;
		y1 = _y1;
	}

	virtual void draw (void) {
		printf ("draw a line from (%d,%d) to (%d,%d)\n", x0, y0, x1, y1);
	}

	~Line (void) {
		printf ("destroying line\n");
	}
};

class Rectangle : public Shape {
private:
	Line *top, *bottom, *left, *right;

public:
	Rectangle (int _x0, int _y0, int _x1, int _y1) : Shape () {
		top = new Line (_x0, _y1, _x1, _y1);
		bottom = new Line (_x0, _y0, _x1, _y0);
		left = new Line (_x0, _y0, _x0, _y1);
		right = new Line (_x1, _y0, _x1, _y1);
	}

	virtual void draw (void) {
		top->draw ();
		bottom->draw ();
		left->draw ();
		right->draw ();
	}

	~Rectangle (void) {
		printf ("destroying rectangle\n");
		delete top;
		delete bottom;
		delete left;
		delete right;
	}
};

class Square : public Rectangle {
public:
	Square (int x0, int y0, int length) 
		: Rectangle (x0, y0, x0+length, y0+length) { }

	~Square (void) {
		printf ("destroying square\n");
	}
};

//class listoshapes : public std::list<Shape*> { }

int main () {
	std::list <Shape *> shapes;
	shapes.push_back (new Rectangle (0, 0, 100, 100));
	shapes.push_back (new Circle (0, 0, 10));
	shapes.push_back (new Square (0, 0, 20));

	std::list<Shape *>::iterator p;
	for (p=shapes.begin(); p!=shapes.end(); p++) (*p)->draw ();
	for (p=shapes.begin(); p!=shapes.end(); p++) delete *p;
	return 0;
}
