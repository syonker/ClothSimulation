#pragma once

#include "Core.h"
#include "Tokenizer.h"
#include <iostream>
#include "Animation.h"
#include "Skeleton.h"

class Player
{

public:

	Player(Animation* newAnimation, Skeleton* newSkeleton);
	~Player();
	void Evaluate();
	void PoseSkel();

	Animation* animation;
	Skeleton* skeleton;

	std::vector<float> pose;
	float t;
	float tStart, tEnd;
	float tInc;

	glm::mat4 rootTranslate;

private:

};