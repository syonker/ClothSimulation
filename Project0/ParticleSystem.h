#pragma once

#include "Core.h"
#include "Particle.h"

class ParticleSystem
{
public:

	ParticleSystem();
	~ParticleSystem();

	void InitializeCloth(unsigned int size);
	void Update(float deltaTime);
	void Draw();

	unsigned int NumParticles;
	
	std::vector<Particle*> Particles;

private:

};

