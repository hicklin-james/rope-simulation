#include "Particle.h"

Particle::Particle(double mass) {
	this->mass = mass;
}


void Particle::addToForce(Vector3 force) {
	this->force += force;
}
	

void Particle::init() {
	force[0] = 0;
	force[1] = 0;
	force[2] = 0;
}


void Particle::calcNew(double dt) {
	vel += (force / mass) * dt;
	pos += vel * dt;
}