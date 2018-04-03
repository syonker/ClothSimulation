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

	UpdateNormals();
	UpdateShaderArrays();

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

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*shaderVerts.size(), shaderVerts.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); 

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*shaderNormals.size(), shaderNormals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*shaderIndices.size(), &shaderIndices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

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


void ParticleSystem::Up() {

	glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.5f,0.0f));
	Transform(T);

}

void ParticleSystem::Down() {

	glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f));
	Transform(T);

}

void ParticleSystem::Left() {

	glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f));
	Transform(T);
}

void ParticleSystem::Right() {

	glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
	Transform(T);

}

void ParticleSystem::Forward() {

	glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f));
	Transform(T);

}

void ParticleSystem::Back() {

	glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f));
	Transform(T);

}

void ParticleSystem::RotateC() {

	glm::mat4 T = glm::rotate(glm::mat4(1.0f), 0.1f,  glm::vec3(0.0f, 1.0f, 0.0f));
	Transform(T);

}

void ParticleSystem::RotateCC() {

	glm::mat4 T = glm::rotate(glm::mat4(1.0f), -0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
	Transform(T);

}

void ParticleSystem::Transform(glm::mat4 T) {

	glm::vec4 pos;

	for (int i = 0; i < NumParticles; i++) {

		if (Particles[i]->fixed) {

			pos = { Particles[i]->Position, 1.0f };
			pos = T * pos;
			Particles[i]->Position = {pos.x,pos.y,pos.z};

			Particles[i]->model->MakeBox((Particles[i]->Position - Particles[i]->offset), (Particles[i]->Position + Particles[i]->offset));

		}

	}

}