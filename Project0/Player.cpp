
#include "Player.h"

Player::Player(Animation* newAnimation, Skeleton* newSkeleton) {

	animation = newAnimation;
	skeleton = newSkeleton;

	for (int i = 0; i < (newSkeleton->numDOFs+3); i++) {
		pose.push_back(0);
	}

	tStart = animation->tStart;
	tEnd = animation->tEnd;
	t = tStart;

	tInc = 0.01f;

}

Player::~Player() {

}

//called every frame
void Player::Evaluate() {

	animation->Evaluate(t, pose);

	PoseSkel();

	t += tInc;

}


//given a std::vector<float> pose, use this to pose the skeleton
void Player::PoseSkel() {

	rootTranslate = { 1,0,0,0,
					   0,1,0,0,
					   0,0,1,0,
					   pose[0],pose[1],pose[2],1 };
	
	int poseIndex = 3;
	for (int i = 0; i < skeleton->joints.size(); i++) {

		skeleton->joints[i]->jointDOF[0]->SetValue(pose[poseIndex]);
		poseIndex++;
		skeleton->joints[i]->jointDOF[1]->SetValue(pose[poseIndex]);
		poseIndex++;
		skeleton->joints[i]->jointDOF[2]->SetValue(pose[poseIndex]);
		poseIndex++;

	}

}