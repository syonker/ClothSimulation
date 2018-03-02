
#include "Joint.h"

Joint::Joint()
{
	//set default values
	offset = {0.0f,0.0f,0.0f};
	boxmin = { 0.0f,0.0f,0.0f };
	boxmax = { 0.0f,0.0f,0.0f };
	pose = { 0.0f,0.0f,0.0f };
	DOFcount = 3;
	L = glm::mat4(1.0f);
	W = glm::mat4(1.0f);

	model = new Model();

	DOF* xDOF = new DOF();
	DOF* yDOF = new DOF();
	DOF* zDOF = new DOF();
	jointDOF.push_back(xDOF);
	jointDOF.push_back(yDOF);
	jointDOF.push_back(zDOF);
	//jointDOF->at(0) = xDOF;
	//jointDOF->at(1) = xDOF;
	//jointDOF->at(2) = xDOF;

	DOFnum = 0;

}

Joint::~Joint()
{
}

bool Joint::Load(Tokenizer* token, std::vector<Joint*>* joints) {

	float min, max;

	token->FindToken("{");
	while (1) {
		char temp[256];
		token->GetToken(temp);
		if (strcmp(temp, "offset") == 0) {
			offset.x = token->GetFloat();
			offset.y = token->GetFloat();
			offset.z = token->GetFloat();
		}
		else if (strcmp(temp, "boxmin") == 0) {
			boxmin.x = token->GetFloat();
			boxmin.y = token->GetFloat();
			boxmin.z = token->GetFloat();
		}
		else if (strcmp(temp, "boxmax") == 0) {
			boxmax.x = token->GetFloat();
			boxmax.y = token->GetFloat();
			boxmax.z = token->GetFloat();
		}
		else if (strcmp(temp, "rotxlimit") == 0) {
			min = token->GetFloat();
			max = token->GetFloat();
			jointDOF[0]->SetMinMax(min, max);
		}
		else if (strcmp(temp, "rotylimit") == 0) {
			min = token->GetFloat();
			max = token->GetFloat();
			jointDOF[1]->SetMinMax(min, max);
		}
		else if (strcmp(temp, "rotzlimit") == 0) {
			min = token->GetFloat();
			max = token->GetFloat();
			jointDOF[2]->SetMinMax(min, max);
		}
		else if (strcmp(temp, "pose") == 0) {
			jointDOF[0]->SetValue(token->GetFloat());
			jointDOF[1]->SetValue(token->GetFloat());
			jointDOF[2]->SetValue(token->GetFloat());
		}
		else if (strcmp(temp, "balljoint") == 0) {
			Joint *jnt = new Joint();
			joints->push_back(jnt);
			jnt->Load(token, joints);
			AddChild(jnt);
		}
		else if (strcmp(temp, "}") == 0) {

			model->MakeBox(boxmin, boxmax);
			return true;
		}
		else {
			token->SkipLine(); // Unrecognized token
		}

	}

}

void Joint::Update(glm::mat4 parentW) {

	float x = jointDOF[0]->GetValue();
	float y = jointDOF[1]->GetValue();
	float z = jointDOF[2]->GetValue();

	glm::mat4 rotX = glm::mat4(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
					glm::vec4(0.0f, cos(x), sin(x), 0.0f),
					glm::vec4(0.0f, -sin(x), cos(x), 0.0f),
					glm::vec4(0.0f,0.0f,0.0f,1.0f));

	glm::mat4 rotY = glm::mat4(glm::vec4(cos(y), 0.0f, -sin(y), 0.0f),
					glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
					glm::vec4(sin(y), 0.0f, cos(y), 0.0f),
					glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	glm::mat4 rotZ = glm::mat4(glm::vec4(cos(z), sin(z), 0.0f, 0.0f),
					glm::vec4(-sin(z), cos(z), 0.0f, 0.0f),
					glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
					glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	glm::mat4 translate = glm::mat4(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
									glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
									glm::vec4(0.0f, 0.0f, 1.0f, 0.0f), 
									glm::vec4(offset, 1.0f));

	//L = glm::mat4(glm::vec4(cos(y)*cos(z), cos(y)*sin(z), -sin(z), 0.0),
	//	glm::vec4(sin(x)*sin(y)*cos(z) - cos(x)*sin(z), sin(x)*sin(y)*sin(z) + cos(x)*cos(z), sin(x)*cos(y), 0.0),
	//	glm::vec4(cos(x)*sin(y)*cos(z) + sin(x)*sin(z), cos(x)*sin(y)*sin(z) - sin(x)*cos(z), cos(x)*cos(y), 0.0),
	//	glm::vec4(offset, 1.0f));

	L = translate * rotZ * rotY * rotX;

	W = parentW * L;

	for (unsigned int i = 0; i < children.size(); i++) {
		
		children[i]->Update(W);

	}


}

void Joint::AddChild(Joint* newChild) {
	
	children.push_back(newChild);

}

void Joint::Draw(const glm::mat4 &viewProjMtx, uint shader) {

	model->Draw(W, viewProjMtx, shader);

	for (unsigned int i = 0; i < children.size(); i++) {

		children[i]->Draw(viewProjMtx, shader);

	}

}



void Joint::makeJointVector(std::vector<Joint*>* joints) {

	joints->push_back(this);

	for (unsigned int i = 0; i < children.size(); i++) {

		children[i]->makeJointVector(joints);

	}

}