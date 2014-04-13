#include "Spring.h"

Spring::Spring(Particle *particle1, Particle *particle2, double constant,
		       double length, double friction) 
{
	this->particle1 = particle1;
	this->particle2 = particle2;
	this->constant = constant;
	this->length = length;
	this->friction = friction;
}

void Spring::simulateSpring() {

	// Define the spring vector as the vector connecting patricle1 and particle2
	Vector3 springVec = particle1->pos - particle2->pos;
	double r = springVec.length();

	Vector3 force;
	
	// Avoid dividing by 0
	if (r != 0) {
		force += (springVec / r) * (r - length) * (-constant);
	}

	force += (Vector3(0,0,0) - (particle1->vel) - particle2->vel) * friction;
	particle1->addToForce(force);
	//force.print("force");
	particle2->addToForce(Vector3(0,0,0)-force);
	//(Vector3(0,0,0)-force).print("Minus force");
}