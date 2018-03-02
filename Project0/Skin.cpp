
#include "Skin.h"

Skin::Skin(Skeleton* skel) {

	skeleton = skel;

}

Skin::~Skin() {

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO);

}


void Skin::BindShader() {


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*shaderVerts.size(), shaderVerts.data(), GL_STATIC_DRAW);

	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*shaderNormals.size(), shaderNormals.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), (GLvoid*)0);


	// We've sent the vertex data over to OpenGL, but there's still something missing.
	// In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*shaderIndices.size(), &shaderIndices[0], GL_STATIC_DRAW);


	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);

}


bool Skin::Load(const char *file) {

	if (file == "") {

		file = "../skeletons/wasp.skin";
	}

	std::cerr << "file: " << file << std::endl;

	Tokenizer* token = new Tokenizer();
	token->Open(file);

	//make vertices
	token->FindToken("positions");
	numV = token->GetFloat();

	//assign positions
	token->FindToken("{");

	float x, y, z;

	for (int count = 1; count <= numV; count++) {

		x = token->GetFloat();
		y = token->GetFloat();
		z = token->GetFloat();

		Vertex* newVertex = new Vertex(x, y, z);

		vertices.push_back(newVertex);

		shaderVerts.push_back(x);
		shaderVerts.push_back(y);
		shaderVerts.push_back(z);

	}

	token->FindToken("}");

	//assign normals
	token->FindToken("normals");
	numV = token->GetFloat();
	token->FindToken("{");


	for (int count = 1; count <= numV; count++) {

		x = token->GetFloat();
		y = token->GetFloat();
		z = token->GetFloat();

		vertices[count - 1]->SetNormal(x, y, z);

		shaderNormals.push_back(x);
		shaderNormals.push_back(y);
		shaderNormals.push_back(z);

	}

	token->FindToken("}");

	//assign attatchments
	token->FindToken("skinweights");
	numV = token->GetFloat();
	token->FindToken("{");

	float jointWeight;
	unsigned int numAttatchments, jointNum;

	for (int count = 1; count <= numV; count++) {

		numAttatchments = token->GetFloat();

		for (int aCount = 1; aCount <= numAttatchments; aCount++) {

			jointNum = token->GetFloat();
			jointWeight = token->GetFloat();

			vertices[count - 1]->joints.push_back(skeleton->joints[jointNum]);
			vertices[count - 1]->weights.push_back(jointWeight);

		}



	}

	token->FindToken("}");


	//save triangles
	token->FindToken("triangles");
	int numT = token->GetFloat();
	token->FindToken("{");

	unsigned int v1, v2, v3;

	for (int count = 1; count <= numT; count++) {

		v1 = token->GetFloat();
		v2 = token->GetFloat();
		v3 = token->GetFloat();

		shaderIndices.push_back(v1);
		shaderIndices.push_back(v2);
		shaderIndices.push_back(v3);

	}

	token->FindToken("}");


	//save binding to joints (need skeleton)
	token->FindToken("bindings");
	int numJ = token->GetFloat();
	token->FindToken("{");
	float ax, ay, az, bx, by, bz, cx, cy, cz, dx, dy, dz;

	for (int count = 1; count <= numJ; count++) {

		token->FindToken("matrix");
		token->FindToken("{");

		ax = token->GetFloat();
		ay = token->GetFloat();
		az = token->GetFloat();
		bx = token->GetFloat();
		by = token->GetFloat();
		bz = token->GetFloat();
		cx = token->GetFloat();
		cy = token->GetFloat();
		cz = token->GetFloat();
		dx = token->GetFloat();
		dy = token->GetFloat();
		dz = token->GetFloat();

		glm::mat4 B = glm::mat4(ax, ay, az, 0.0f, bx, by, bz, 0.0f, cx, cy, cz, 0.0f, dx, dy, dz, 1.0f);

		skeleton->joints[count - 1]->Binv = glm::inverse(B);

		token->FindToken("}");

	}

	token->FindToken("}");


	// Finish
	token->Close();

	BindShader();

	return true;

}

void Skin::Update(glm::mat4 parentW) {

	glm::vec4 updatedVec;
	Vertex* currV;

	//for each vertex on the skin
	for (int i = 0; i < numV; i++) {

		updatedVec = { 0,0,0,0 };

		currV = vertices[i];

		//for each joint it is attatched to
		for (int j = 0; j < currV->joints.size(); j++) {

			updatedVec += (currV->weights[j] * ((currV->joints[j]->W)*(currV->joints[j]->Binv)*(glm::vec4(currV->position, 1.0f))));

		}

		//prepare the array that will be used for drawing
		shaderVerts[(3 * i)] = updatedVec.x;
		shaderVerts[(3 * i) + 1] = updatedVec.y;
		shaderVerts[(3 * i) + 2] = updatedVec.z;

	}


	glm::vec4 updatedNormal;
	//Vertex* currV;
	glm::vec3 currNormal;
	glm::mat4 M;

	//for each normal on the skin
	for (int i = 0; i < numV; i++) {

		updatedNormal = { 0,0,0,0 };

		currV = vertices[i];

		currNormal = currV->normal;

		//for each joint it is attatched to
		for (int j = 0; j < currV->joints.size(); j++) {

			M = (currV->joints[j]->W)*(currV->joints[j]->Binv);
			//M = glm::inverse(M);
			//M = glm::transpose(M);

			updatedNormal += (currV->weights[j] * (M*(glm::vec4(currNormal, 0.0f))));

		}

		updatedNormal = glm::normalize(updatedNormal);

		//prepare the array that will be used for drawing
		shaderNormals[(3 * i)] = updatedNormal.x;
		shaderNormals[(3 * i) + 1] = updatedNormal.y;
		shaderNormals[(3 * i) + 2] = updatedNormal.z;


	}


}



void Skin::Draw(const glm::mat4 &viewProjMtx, uint shader) {

	//send things to shader
	// Set up shader
	glUseProgram(shader);

	glm::mat4 modelMtx = glm::mat4(1.0f);

	glUniformMatrix4fv(glGetUniformLocation(shader, "ModelMtx"), 1, false, (float*)&modelMtx);

	glm::mat4 mvpMtx = viewProjMtx * modelMtx;
	glUniformMatrix4fv(glGetUniformLocation(shader, "ModelViewProjMtx"), 1, false, (float*)&mvpMtx);

	glm::vec3 ambient = { 1,1,1 };

	//send over ambient color
	glUniform3fv(glGetUniformLocation(shader, "AmbientColor"), 1, &ambient[0]);

	//send over arrays and draw
	glBindVertexArray(VAO);

	//send over updated positions
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*shaderVerts.size(), shaderVerts.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//send over updated normals
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*shaderNormals.size(), shaderNormals.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//draw
	glDrawElements(GL_TRIANGLES, (GLsizei)shaderIndices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glUseProgram(0);

}

