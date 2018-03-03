#pragma once

#include "Core.h"
#include "ParticleSystem.h"

class Player
{

public:

	Player(ParticleSystem* system, float tstart, float inc, float tend);
	~Player();

	void Update();
	void Reset();
	void SetActive(bool toggle);

	ParticleSystem* System;
	float time;
	float tStart;
	float tEnd;
	float increment;
	bool active;

};

