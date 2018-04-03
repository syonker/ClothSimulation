#include "SpringDamper.h"



SpringDamper::SpringDamper(Particle* p1, Particle* p2)
{
	P1 = p1;
	P2 = p2;

	L0 = glm::distance(P2->Position, P1->Position);

	Ks = 500.0f;
	Kd = 20.0f;
	
}


SpringDamper::~SpringDamper()
{
}

void SpringDamper::ComputeForce() {

	glm::vec3 E = P2->Position - P1->Position;

	float L = glm::length(E);

	E = glm::normalize(E);

	float V1 = glm::dot(E, P1->Velocity);
	float V2 = glm::dot(E, P2->Velocity);

	float force = (-Ks * (L0 - L)) - (Kd * (V1 - V2));

	P1->ApplyForce(force*E);
	P2->ApplyForce(-force*E);

}
