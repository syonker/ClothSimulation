
#pragma once

#include "Core.h"
#include "DOF.h"
#include "Tokenizer.h"
#include "Model.h"
#include <iostream>


class Joint
{
public:
	Joint();
	~Joint();

	void Update(glm::mat4 parentW);
	void AddChild(Joint* newChild);
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
	bool Load(Tokenizer* token, std::vector<Joint*>* joints);


	void makeJointVector(std::vector<Joint*>* joints);

	std::vector<Joint*> children;
	std::vector<DOF*> jointDOF;
	glm::vec3 offset;
	glm::vec3 boxmin;
	glm::vec3 boxmax;
	glm::vec3 pose;
	glm::mat4 Binv;
	int DOFcount;
	int DOFnum;
	glm::mat4 L;
	glm::mat4 W;
	Model* model;


private:

};

