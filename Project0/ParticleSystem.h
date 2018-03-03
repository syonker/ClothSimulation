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
	void SetParticleNormals();
	void BindShader();

	unsigned int NumParticles;
	std::vector<Particle*> Particles;
	std::vector<Triangle*> Triangles;
	std::vector<SpringDamper*> SpringDampers;
	bool particleView;

	std::vector<float> shaderVerts;
	std::vector<unsigned int> shaderIndices;
	std::vector<float> shaderNormals;
	uint shader;
	uint VBO, VBO2, VAO, EBO;

private:

};

