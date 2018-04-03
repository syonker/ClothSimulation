#include "Triangle.h"



Triangle::Triangle(Particle* p1, Particle* p2, Particle* p3)
{
	P1 = p1;
	P2 = p2;
	P3 = p3;

	UpdateNormal();

	vAir = {0,0,-1.0f};

}


Triangle::~Triangle()
{
}

void Triangle::UpdateNormal() {

	Normal = glm::cross((P2->Position - P1->Position), (P3->Position - P1->Position));
	Normal = glm::normalize(Normal);

}

void Triangle::AerodynamicForce() {

	float density = 1.0f;
	float drag = 0.5f;


	glm::vec3 vSurface = (P1->Velocity + P2->Velocity + P3->Velocity) * (1.0f/3.0f);
	glm::vec3 Veloctiy = vSurface - vAir;
	glm::vec3 E = -glm::normalize(Veloctiy);

	glm::vec3 temp = glm::cross((P2->Position - P1->Position), (P3->Position - P1->Position));

	float area = (0.5f)*(glm::length(temp));
	area = area * glm::dot(glm::normalize(Veloctiy),Normal);

	glm::vec3 force = (-0.5f) * density * ((glm::length(Veloctiy))*(glm::length(Veloctiy))) * drag * area * Normal;
	
	P1->ApplyForce((force / 3.0f));
	P2->ApplyForce((force / 3.0f));
	P3->ApplyForce((force / 3.0f));

}
