#pragma once

#include "Core.h"

class Particle {

public:

	Particle();
	~Particle();

	void Update(float deltaTime);
	void Draw();
	void ApplyForce(glm::vec3 force);

	float Mass;
	glm::vec3 Position;
	glm::vec3 Velocity;
	glm::vec3 Force;

private:

};