#include "GraphicsMath.h"

class Particle {

public:
	// Mass of the particle
	double mass;
	// Position of the particle
	Vector3 pos;
	// Velocity of the particle
	Vector3 vel;
	// Total force that is acting on the particle
	Vector3 force;

	// Constructor //
	Particle(double mass);
	// Add to the total force being applied to this particle //
	void addToForce(Vector3 force);
	// Initializer //
	void init();
	// Calculate new position and velocity according to dt //
	void calcNew(double dt);
 
};