#pragma once

#include "Core.h"
#include "Particle.h"
#include "Triangle.h"
#include <iostream>
#include "SpringDamper.h"

class ParticleSystem
{
public:

	ParticleSystem();
	~ParticleSystem();

	void InitializeCloth(unsigned int size, float xOffset, float yOffset);
	void Update(float deltaTime);
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
	void ResetParticles();

	unsigned int NumParticles;
	std::vector<Particle*> Particles;
	std::vector<Triangle*> Triangles;
	std::vector<SpringDamper*> SpringDampers;

private:

};

