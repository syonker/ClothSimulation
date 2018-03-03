
#include "Particle.h"

Particle::Particle() {


}

Particle::~Particle() {


}

void Particle::Update(float deltaTime) {

	//Compute Acceleration
	glm::vec3 acc = (1.0f / Mass) * Force;

	//Compute new position and velocity
	Velocity += acc * deltaTime;
	Position += Velocity * deltaTime;

	//Zero out force vector
	Force = {0.0f,0.0f,0.0f};



}

void Particle::Draw() {

}

void Particle::ApplyForce(glm::vec3 force) {

	Force += force;

}