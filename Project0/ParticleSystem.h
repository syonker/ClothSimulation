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
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
	void ResetParticles();

	unsigned int NumParticles;
	std::vector<Particle*> Particles;

private:

};

