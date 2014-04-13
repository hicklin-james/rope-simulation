#include "Simulation.h"
#include <stdio.h>
#include <iostream>

Simulation::Simulation(int totalParticles, double mass, double springConstant, double springLength,
			   double springFriction, Vector3 gravity, double airFriction, double groundRepulsion,
			   double groundFriction, double groundAbsorption, double groundHeight)
{

	this->totalParticles = totalParticles;

	particles = new Particle*[totalParticles];
	for (int i = 0; i < totalParticles; i++) {
		particles[i] = new Particle(mass);
	}


	this->gravity = gravity;
	this->airFriction = airFriction;
	this->groundFriction = groundFriction;
	this->groundRepulsion = groundRepulsion;
	this->groundAbsorption = groundAbsorption;
	this->groundHeight = groundHeight;

	for (int r = 0; r < totalParticles; r++) {
		particles[r]->pos[0] = r * springLength;
		particles[r]->pos[1] = 0;
		particles[r]->pos[2] = 0;
	}

	springs = new Spring*[totalParticles-1];

	for (int s = 0; s < totalParticles-1; s++) {
		springs[s] = new Spring(particles[s], particles[s+1],
								springConstant, springLength, 
								springFriction);
	}
}

void Simulation::init() {
	for (int i = 0; i < totalParticles; i++) {
		particles[i]->init();
	}
}

void Simulation::release()
{
	for (int i = 0; i < totalParticles; i++) {
		delete(particles[i]);
		particles[i] = NULL;
	}

	delete(particles);
	particles = NULL;
}

void Simulation::solve() 
{

	for (int i = 0; i < totalParticles-1; i++) {
		springs[i]->simulateSpring();
	}

	for (int s = 0; s < totalParticles; s++) {
		particles[s]->addToForce(gravity * particles[s]->mass);
		particles[s]->addToForce((Vector3(0,0,0)-(particles[s]->vel)) * airFriction);

		if (particles[s]->pos[1] < groundHeight) {
			//std::cout << particles[s]->pos[1] << "\n";
			Vector3 v;

			v = particles[s]->vel;
			v[1] = 0;

			particles[s]->addToForce((Vector3(0,0,0)-v) * groundFriction);

			v = particles[s]->vel;
			v[0] = 0;
			v[2] = 0;

			if (v[1] < 0) {
				particles[s]->addToForce((Vector3(0,0,0)-v) * groundAbsorption);
			}

			Vector3 force = Vector3(0, groundRepulsion, 0) * (groundHeight - particles[s]->pos[1]);
			particles[s]->addToForce(force);
		}
	}
}

void Simulation::simulate(double dt) 
{
	for (int i = 0; i < totalParticles; i++) {
		particles[i]->calcNew(dt);
	}

	ropeConnectionPos += ropeConnectionVel * dt;
	//std::cout << ropeConnectionPos << "\n";
	if (ropeConnectionPos[1] < groundHeight) {
		ropeConnectionPos[1] = groundHeight;
		ropeConnectionVel[1] = 0;
	}

	particles[0]->pos = ropeConnectionPos;
	particles[0]->vel = ropeConnectionVel;
}

void Simulation::setAirFriction(double n) {
	this->airFriction = n;
}

void Simulation::setRopeConnectionVel(Vector3 ropeConnectionVel) {
	this->ropeConnectionVel = ropeConnectionVel;
}

void Simulation::setGravity(Vector3 gravity) {
	this->gravity = gravity;
}

void Simulation::setGroundRepulsion(double n) {
	this->groundRepulsion = n;
}

void Simulation::setGroundAbsorption(double n) {
	this->groundAbsorption = n;
}

void Simulation::setGroundFriction(double n) {
	this->groundFriction = n;
}

Particle* Simulation::getParticle(int i) {
	if (i < 0 || i >= totalParticles)
		return NULL;
	return particles[i];
}

void Simulation::operate(double dt) {
	init();
	solve();
	simulate(dt);
}
