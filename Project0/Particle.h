#pragma once

#include "Core.h"
#include "Model.h"

class Particle {

public:

	Particle(float x, float y, float z);
	~Particle();

	void Update(float deltaTime);
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
	void ApplyForce(glm::vec3 force);
	void Reset();

	Model* model;
	float Mass;
	glm::vec3 Position;
	glm::vec3 Velocity;
	glm::vec3 Force;
	glm::vec3 ogPosition;
	glm::vec3 offset;
	bool fixed;

private:

};