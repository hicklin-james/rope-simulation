#include "Particle.h"
#include <stdio.h>
#include <iostream>

class Spring {

public:
	// The two end points of the spring, since it connects two particles
	Particle *particle1;
	Particle *particle2;

	// Spring constant
	double constant;
	// Spring length
	double length;
	// Spring friction
	double friction;
	// Constructor
	Spring(Particle *particle1, Particle *particle2, double constant,
		   double length, double friction);
	void simulateSpring();

};