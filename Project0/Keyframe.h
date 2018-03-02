#pragma once

#include "Core.h"
#include "Tokenizer.h"
#include <iostream>

class Keyframe
{

public:

	Keyframe();
	~Keyframe();

	bool Load(Tokenizer* token);

	float time;
	float value;
	float tangIn, tangOut;
	char ruleIn[256];
	char ruleOut[256];
	float a, b, c, d;

private:

};