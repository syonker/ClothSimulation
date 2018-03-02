
#include "Skeleton.h"



Skeleton::Skeleton()
{

	root = NULL;

	increment = 0.1f;

}

Skeleton::~Skeleton()
{
}


bool Skeleton::Load(const char *file) {
	
	if (file == "") {

		file = "../skeletons/wasp.skel";
	}

	std::cerr << "file: " << file << std::endl;

	Tokenizer* token = new Tokenizer();
	//token->Open(file, "skel");
	token->Open(file);
	token->FindToken("balljoint");
	// Parse tree
	root = new Joint();
	joints.push_back(root);
	root->Load(token, &joints);

	numJoints = joints.size();

	std::cerr << "Number of Joints: " << numJoints << std::endl;

	numDOFs = numJoints * 3;


	//makeJointVector();
	//std::cerr << "Size of Joints: " << joints.size() << std::endl;

	jointNum = 0;

	activeJoint = root;
	activeDOF = root->jointDOF[0];

	


	// Finish
	token->Close();
	return true;
}


void Skeleton::Update(glm::mat4 parentW) {

	root->Update(parentW);

}

void Skeleton::Draw(const glm::mat4 &viewProjMtx, uint shader) {

	root->Draw(viewProjMtx, shader);

}



void Skeleton::makeJointVector() {


	root->makeJointVector(&joints);

}

void Skeleton::upSelection() {

	

	Joint* currJoint = joints[jointNum];

	std::cerr << "DOFnum: " << currJoint->DOFnum << " count: " << currJoint->DOFcount << std::endl;

	if (currJoint->DOFnum < currJoint->DOFcount-1) {

		//do the thing at DOFnum
		activeDOF = currJoint->jointDOF[currJoint->DOFnum];

		currJoint->DOFnum++;

	}
	else {

		joints[jointNum]->model->ambient = { 1,1,0 };

		if (jointNum < joints.size()-1) {

			jointNum++;

		}
		else {

			jointNum = 0;

		}
		

		joints[jointNum]->DOFnum = 0;
		activeJoint = joints[jointNum];


	}


}

void Skeleton::downSelection() {

	std::cerr << "downSelection" << std::endl;

	Joint* currJoint = joints[jointNum];

	if (currJoint->DOFnum > 0) {

		//do the thing at DOFnum
		activeDOF = currJoint->jointDOF[currJoint->DOFnum];

		currJoint->DOFnum--;

	}
	else {

		joints[jointNum]->model->ambient = { 1,1,0 };

		if (jointNum > 0) {

			jointNum--;

		}
		else {

			jointNum = joints.size()-1;

		}

		joints[jointNum]->DOFnum = 2;
		activeJoint = joints[jointNum];


	}

}

void Skeleton::incDOF() {

	activeDOF->Add(increment);

}

void Skeleton::decDOF() {

	activeDOF->Add(-increment);

}