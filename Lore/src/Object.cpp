#include "Object.h"

#include <iostream>

using std::cout;
using std::endl;
using glm::mat4;

using namespace LORE;

Object::Object(): m__mesh() {
	cout << this << " [Object] constructor" << endl;
}

Object::~Object() {
	cout << this << " [Object] destructor" << endl;
}

void Object::render(Node* renderer, mat4 projection, mat4 view, mat4 model, GLuint environmentMapID) {
    //cout << this << "[Object] render" << endl;
    mat4 local_model = getModel(model);

	if(m__mesh) {
		m__mesh->render(renderer, projection, view, local_model, environmentMapID);
    } else {
        cout << "[Object]: mesh not set !" << endl;
    }

    Node::render(renderer, projection, view, model, environmentMapID);
}

void Object::setMesh(Mesh* mesh) {
    if(!mesh) {
        cout << "Warning !!! Setting NULL Mesh !!!" << endl;
    }
    m__mesh = mesh;
}
