
#include "Rig.h"

Rig::Rig() {
	skeleton = new Skeleton();
	skin = new Skin(skeleton);
}

Rig::~Rig() {
	delete skeleton;
	delete skin;
}

bool Rig::Load(const char *file1, const char *file2) {
	if (skeleton->Load(file1) && skin->Load(file2)) {
		return true;
	}
	else {
		return false;
	}
}

void Rig::Update(glm::mat4 parentW) {
	skeleton->Update(parentW);
	skin->Update(parentW);
}

void Rig::Draw(const glm::mat4 &viewProjMtx, uint shader, bool drawSkel, bool drawSkin) {
	if (drawSkel) {
		float color = 1.0f;
		color = ((float)(skeleton->activeJoint->DOFnum + 1) / (float)(skeleton->activeJoint->DOFcount));
		skeleton->activeJoint->model->ambient = { color,color,color };
		skeleton->Draw(viewProjMtx, shader);
	}
	if (drawSkin) {
		skin->Draw(viewProjMtx, shader);
	}
}