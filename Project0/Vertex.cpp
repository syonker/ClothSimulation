
#include "Vertex.h"

Vertex::Vertex(float x, float y, float z) {

	position = { x,y,z };

}

Vertex::~Vertex() {


}

void Vertex::SetNormal(float x, float y, float z) {

	normal = {x,y,z};

}