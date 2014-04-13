#include "Spring.h"
#include <stdlib.h>
#include <stdint.h>

class Simulation {

public:
	// The total number of particles in the simulation
	int totalParticles;
	Particle** particles;
	Spring** springs;
	Vector3 gravity;
	Vector3 ropeConnectionPos;
	Vector3 ropeConnectionVel;
	double groundRepulsion;
	double groundFriction;
	double groundAbsorption;
	double groundHeight;
	double airFriction;

	// Constructor
	Simulation(int totalParticles, double mass, double springConstant, double springLength,
			   double springFriction, Vector3 gravity, double airFriction, double groundRepulsion,
			   double groundFriction, double groundAbsorption, double groundHeight);

	void init();
	// Delete the particles
	void release();

	void solve();

	void simulate(double dt);

	void setAirFriction(double n);
	void setGroundRepulsion(double n);
	void setGroundAbsorption(double n);
	void setGroundFriction(double n);
	void setRopeConnectionVel(Vector3 ropeConnectionVel);
	void setGravity(Vector3 gravity);
	// Get the particle at index i
	Particle* getParticle(int i);

	void operate(double dt);

};