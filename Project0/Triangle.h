#pragma once

#include "Core.h"
#include "Particle.h"

class Triangle
{

public:

	Triangle(Particle* p1, Particle* p2, Particle* p3);
	~Triangle();

	void UpdateNormal();
	void AerodynamicForce();

	Particle* P1;
	Particle* P2;
	Particle* P3;

	glm::vec3 Normal;
	glm::vec3 vAir;

private:

};

