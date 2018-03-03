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

			//little offset to keep it from staying flat
			if (i == 0 && j == 0) {

				Particle* newP = new Particle(i + xOffset, j + yOffset, 1.0f);
				Particles.push_back(newP);

			}
			else if (i == size-1 && j == size-1) {

				Particle* newP = new Particle(i + xOffset, j + yOffset, 1.0f);
				newP->fixed = true;
				Particles.push_back(newP);

			}
			else {

				Particle* newP = new Particle(i + xOffset, j + yOffset, 0);
				Particles.push_back(newP);

			}


		}

	}

	NumParticles = size * size;


	//build triangles and springdampers
	for (int i = 0; i < NumParticles-1; i++) {

		//everything but bottom edge of cloth
		if (i < NumParticles - size) {

			//anywhere but right edge of cloth
			if (((i + 1) % size) != 0) {

				Triangle* newT1 = new Triangle(Particles[i], Particles[i + 1], Particles[i + size]);
				Triangles.push_back(newT1);

				//all edges
				SpringDamper* newSD1 = new SpringDamper(Particles[i], Particles[i + 1]);
				SpringDampers.push_back(newSD1);
				SpringDamper* newSD2 = new SpringDamper(Particles[i + 1], Particles[i + size]);
				SpringDampers.push_back(newSD2);
				SpringDamper* newSD3 = new SpringDamper(Particles[i], Particles[i + size]);
				SpringDampers.push_back(newSD3);

				//other diagonal
				SpringDamper* newSD4 = new SpringDamper(Particles[i], Particles[i + size + 1]);
				SpringDampers.push_back(newSD4);

				//above technique will get all but bottom and right borders

				Triangle* newT2 = new Triangle(Particles[i + 1], Particles[i + size], Particles[i + size + 1]);
				Triangles.push_back(newT2);


			}
			else {

				//will account for right edge
				SpringDamper* newSD5 = new SpringDamper(Particles[i], Particles[i + size]);
				SpringDampers.push_back(newSD5);


			}

		}
		//for the bottom edge
		else {

			SpringDamper* newSD6 = new SpringDamper(Particles[i], Particles[i + 1]);
			SpringDampers.push_back(newSD6);


		}

	}

	std::cerr << "NumSD: " << SpringDampers.size() << std::endl;

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

	for (int i = 0; i < SpringDampers.size(); i++) {

		SpringDampers[i]->ComputeForce();
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