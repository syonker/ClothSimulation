

#pragma once

#include "Core.h"
#include "Joint.h"
#include "Tokenizer.h"
#include <iostream>

class Vertex
{

public:

	Vertex(float x,float y,float z);
	~Vertex();

	void SetNormal(float x, float y, float z);

	glm::vec3 position;
	glm::vec3 normal;

	std::vector<Joint*> joints;
	std::vector<float> weights;


private:


};