

#pragma once

#include "Core.h"
#include "Joint.h"
#include "Tokenizer.h"
#include <iostream>


class Skeleton
{
public:
	Skeleton();
	~Skeleton();

	bool Load(const char *file);
	void Update(glm::mat4 parentW);
	void Draw(const glm::mat4 &viewProjMtx, uint shader);

	void makeJointVector();
	void upSelection();
	void downSelection();

	void incDOF();
	void decDOF();

	Joint* root;
	Joint* activeJoint;
	DOF* activeDOF;
	int jointNum;
	float increment;
	std::vector<Joint*> joints;

	unsigned int numJoints;
	unsigned int numDOFs;

private:

};

