#pragma once

#include "Core.h"
#include "Particle.h"

class SpringDamper
{

public:

	SpringDamper(Particle* p1, Particle* p2);
	~SpringDamper();

	void ComputeForce();

	//Spring Constant
	float Ks = 0.0f;
	//Damping Factor
	float Kd = 0.0f;
	//Rest Length
	float L0;

	Particle* P1;
	Particle* P2;


private:

};

