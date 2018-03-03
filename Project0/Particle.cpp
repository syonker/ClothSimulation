
#include "Particle.h"

Particle::Particle(float x, float y, float z) {

	Position = { x,y,z };
	ogPosition = { x,y,z };
	Velocity = { 0,0,0 };
	Force = { 0,0,0 };
	Mass = 1.0f;

	model = new Model();
	offset = {0.1f,0.1f,0.1f};
	model->MakeBox(Position - offset, Position + offset);


}

Particle::~Particle() {


}

void Particle::Update(float deltaTime) {

	//Compute Acceleration
	glm::vec3 acc = (1.0f / Mass) * Force;

	//Compute new position and velocity
	Velocity += acc * deltaTime;
	Position += Velocity * deltaTime;

	//Use position to update models position
	model->MakeBox(Position - offset, Position + offset);

	//Zero out force vector
	Force = {0.0f,0.0f,0.0f};



}

void Particle::Draw(const glm::mat4 &viewProjMtx, uint shader) {

	model->Draw(glm::mat4(1.0f), viewProjMtx, shader);

}

void Particle::ApplyForce(glm::vec3 force) {

	Force += force;

}

void Particle::Reset() {

	Position = ogPosition;
	Velocity = { 0,0,0 };
	Force = { 0,0,0 };
	model->MakeBox(Position - offset, Position + offset);

}