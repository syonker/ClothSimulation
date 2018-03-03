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
	float Ks;
	//Damping Factor
	float Kd;
	//Rest Length
	float L0;

	Particle* P1;
	Particle* P2;


private:

};

