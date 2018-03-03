#include "Triangle.h"



Triangle::Triangle(Particle* p1, Particle* p2, Particle* p3)
{
	P1 = p1;
	P2 = p2;
	P3 = p3;

	UpdateNormal();

}


Triangle::~Triangle()
{
}

void Triangle::UpdateNormal() {

	Normal = glm::cross((P2->Position - P1->Position), (P3->Position - P1->Position));
	Normal = glm::normalize(Normal);

}

void Triangle::AerodynamicForce() {

	glm::vec3 vAir = {0.0f,0.0f,-1.0f};
	float density = 1.0f;
	float drag = 0.5f;


	glm::vec3 vSurface = (P1->Velocity + P2->Velocity + P3->Velocity) * (1.0f/3.0f);
	glm::vec3 Veloctiy = vSurface - vAir;
	glm::vec3 E = (Veloctiy) * (-1.0f / (Veloctiy.length));

	float area = (0.5f)*((glm::cross((P2->Position - P1->Position), (P3->Position - P1->Position))).length);
	area = area * (glm::dot(Veloctiy, Normal) / Veloctiy.length);

	glm::vec3 force = (-0.5f) * density * ((Veloctiy.length)*(Veloctiy.length)) * drag * area * Normal;
	
	P1->ApplyForce((force / 3.0f));
	P2->ApplyForce((force / 3.0f));
	P3->ApplyForce((force / 3.0f));

}
