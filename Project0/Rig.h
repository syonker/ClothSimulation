#pragma once

#include "Core.h"
#include "Tokenizer.h"
#include <iostream>
#include "Skin.h"
#include "Skeleton.h"

class Rig
{

public:

	Rig();
	~Rig();

	bool Load(const char *file1, const char *file2);
	void Update(glm::mat4 parentW);
	void Draw(const glm::mat4 &viewProjMtx, uint shader, bool drawSkel, bool drawSkin);

	Skeleton* skeleton;
	Skin* skin;

private:

};