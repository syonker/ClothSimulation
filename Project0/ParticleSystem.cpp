#include "ParticleSystem.h"



ParticleSystem::ParticleSystem()
{
	particleView = false;
}


ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::InitializeCloth(unsigned int size, float xOffset, float yOffset) {

	for (int i = 0; i < size; i++) {

		for (int j = 0; j < size; j++) {

			//fix row
			if (j == size-1) {

				Particle* newP = new Particle(i + xOffset, j + yOffset, 0, Particles.size());
				newP->fixed = true;
				Particles.push_back(newP);

			}
			else {

				Particle* newP = new Particle(i + xOffset, j + yOffset, 0, Particles.size());
				Particles.push_back(newP);

			}


		}

	}

	NumParticles = size * size;


	//build triangles and springdampers
	for (int i = 0; i < NumParticles-1; i++) {

		//everything but bottom edge of cloth
		if (i < NumParticles - size) {

			//anywhere but right edge of cloth
			if (((i + 1) % size) != 0) {

				Triangle* newT1 = new Triangle(Particles[i], Particles[i + 1], Particles[i + size]);
				Triangles.push_back(newT1);

				//all edges
				SpringDamper* newSD1 = new SpringDamper(Particles[i], Particles[i + 1]);
				SpringDampers.push_back(newSD1);
				SpringDamper* newSD2 = new SpringDamper(Particles[i + 1], Particles[i + size]);
				SpringDampers.push_back(newSD2);
				SpringDamper* newSD3 = new SpringDamper(Particles[i], Particles[i + size]);
				SpringDampers.push_back(newSD3);

				//other diagonal
				SpringDamper* newSD4 = new SpringDamper(Particles[i], Particles[i + size + 1]);
				SpringDampers.push_back(newSD4);

				//above technique will get all but bottom and right borders

				Triangle* newT2 = new Triangle(Particles[i + 1], Particles[i + size], Particles[i + size + 1]);
				Triangles.push_back(newT2);


			}
			else {

				//will account for right edge
				SpringDamper* newSD5 = new SpringDamper(Particles[i], Particles[i + size]);
				SpringDampers.push_back(newSD5);


			}

		}
		//for the bottom edge
		else {

			SpringDamper* newSD6 = new SpringDamper(Particles[i], Particles[i + 1]);
			SpringDampers.push_back(newSD6);


		}

	}

	//little offset to keep it from staying flat
	glm::vec3 initialOffset = { 0,0,-0.0000001f };
	Particles[0]->Position = Particles[0]->Position + initialOffset;
	Particles[0]->ogPosition = Particles[0]->Position;

	//compute normals
	UpdateNormals();

	//fill the shader arrays to the correct size
	for (int i = 0; i < NumParticles; i++) {
		shaderVerts.push_back(0);
		shaderVerts.push_back(0);
		shaderVerts.push_back(0);
		shaderNormals.push_back(0);
		shaderNormals.push_back(0);
		shaderNormals.push_back(0);
	}
	for (int i = 0; i < Triangles.size(); i++) {

		shaderIndices.push_back(Triangles[i]->P1->index);
		shaderIndices.push_back(Triangles[i]->P2->index);
		shaderIndices.push_back(Triangles[i]->P3->index);

	}

	UpdateShaderArrays();
	BindShader();

}

void ParticleSystem::Update(float deltaTime) {

	//Compute forces
	glm::vec3 gravity = {0.0f, -9.8f, 0.0f};
	glm::vec3 force;


	for (int i = 0; i < NumParticles; i++) {
		
		//f = mg
		force = gravity * Particles[i]->Mass;
		Particles[i]->ApplyForce(force);

	}

	for (int i = 0; i < SpringDampers.size(); i++) {

		SpringDampers[i]->ComputeForce();
	}

	for (int i = 0; i < Triangles.size(); i++) {

		Triangles[i]->AerodynamicForce();
	}

	//Integrate for position
	for (int i = 0; i < NumParticles; i++) {

		Particles[i]->Update(deltaTime);

	}

	//Sparkly if include this
	UpdateNormals();

	UpdateShaderArrays();

}


void ParticleSystem::UpdateShaderArrays() {


	for (int i = 0; i < NumParticles; i++) {

		shaderVerts[(3 * i)] = Particles[i]->Position.x;
		shaderVerts[(3 * i) + 1] = Particles[i]->Position.y;
		shaderVerts[(3 * i) + 2] = Particles[i]->Position.z;

		shaderNormals[(3 * i)] = Particles[i]->Normal.x;
		shaderNormals[(3 * i) + 1] = Particles[i]->Normal.y;
		shaderNormals[(3 * i) + 2] = Particles[i]->Normal.z;
	}


}


void ParticleSystem::Draw(const glm::mat4 &viewProjMtx, uint shader) {

	if (particleView) {

		for (int i = 0; i < NumParticles; i++) {

			Particles[i]->Draw(viewProjMtx, shader);

		}

	}
	else {

		//send things to shader
		// Set up shader
		glUseProgram(shader);

		glDisable(GL_CULL_FACE);

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

}

void ParticleSystem::ResetParticles() {

	for (int i = 0; i < NumParticles; i++) {

		Particles[i]->Reset();

	}

}

void ParticleSystem::UpdateNormals() {

	for (int i = 0; i < Triangles.size(); i++) {

		Triangles[i]->UpdateNormal();

	}

	for (int i = 0; i < NumParticles; i++) {

		Particles[i]->Normal = {0,0,0};

	}

	for (int i = 0; i < Triangles.size(); i++) {

		Triangles[i]->P1->Normal += Triangles[i]->Normal;
		Triangles[i]->P2->Normal += Triangles[i]->Normal;
		Triangles[i]->P3->Normal += Triangles[i]->Normal;

	}

	for (int i = 0; i < NumParticles; i++) {

		Particles[i]->Normal = glm::normalize(Particles[i]->Normal);

	}
}


void ParticleSystem::BindShader() {


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

void ParticleSystem::ToggleParticleView() {

	if (particleView) {
		particleView = false;
	}
	else {
		particleView = true;
	}

}

void ParticleSystem::ChangeAir(bool up) {

	float increment;
	glm::vec3 inc;

	if (!up) {
		increment = 1.0f;
	}
	else {
		increment = -1.0f;
	}

	inc = { 0.0f,0.0f,increment };

	for (int i = 0; i < Triangles.size(); i++) {

		Triangles[i]->vAir += inc;

	}


}