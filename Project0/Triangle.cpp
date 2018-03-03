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
