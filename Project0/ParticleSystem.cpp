#include "ParticleSystem.h"



ParticleSystem::ParticleSystem()
{

}


ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::InitializeCloth(unsigned int size, float xOffset, float yOffset) {

	for (int i = 0; i < size; i++) {

		for (int j = 0; j < size; j++) {

			Particle* newP = new Particle(i + xOffset,j + yOffset,0);
			Particles.push_back(newP);

		}

	}

	NumParticles = size * size;


	//build triangles
	for (int i = 0; i < NumParticles - size; i++) {

		if (((i+1) % size) != 0) {

			Triangle* newT = new Triangle(Particles[i], Particles[i+1], Particles[i+size]);
			Triangles.push_back(newT);

			Triangle* newT = new Triangle(Particles[i + 1], Particles[i + size], Particles[i + size + 1]);
			Triangles.push_back(newT);

		}

	}

	//std::cerr << "NumT: " << Triangles.size() << std::endl;

}

void ParticleSystem::Update(float deltaTime) {

	//Compute forces
	glm::vec3 gravity = {0.0f, -9.8f, 0.0f};
	glm::vec3 force;

	for (int i = 0; i < NumParticles; i++) {
		
		//f = mg
		force = gravity * Particles[i]->Mass;
		Particles[i]->ApplyForce(force);

	}

	//Integrate for position
	for (int i = 0; i < NumParticles; i++) {

		Particles[i]->Update(deltaTime);

	}


}

void ParticleSystem::Draw(const glm::mat4 &viewProjMtx, uint shader) {

	for (int i = 0; i < NumParticles; i++) {

		Particles[i]->Draw(viewProjMtx, shader);

	}

}

void ParticleSystem::ResetParticles() {

	for (int i = 0; i < NumParticles; i++) {

		Particles[i]->Reset();

	}

}