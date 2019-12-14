#include "Cube.h"

#include <iostream>

using std::cout;
using std::endl;
using std::vector;

using namespace LORE;

Cube::Cube(): Mesh() {
    cout << this << " [Cube] constructor" << endl;

    vector<float> vertices = {
    -0.5,  0.5,  0.5,   -0.5, -0.5,  0.5,   0.5, -0.5,  0.5,
    -0.5,  0.5,  0.5,    0.5, -0.5,  0.5,   0.5,  0.5,  0.5,

    -0.5,  0.5, -0.5,    0.5, -0.5, -0.5,  -0.5, -0.5, -0.5,
    -0.5,  0.5, -0.5,    0.5,  0.5, -0.5,   0.5, -0.5, -0.5,

    -0.5,  0.5, -0.5,   -0.5, -0.5, -0.5,  -0.5, -0.5,  0.5,
    -0.5,  0.5, -0.5,   -0.5, -0.5,  0.5,  -0.5,  0.5,  0.5,

     0.5,  0.5, -0.5,    0.5, -0.5,  0.5,   0.5, -0.5, -0.5,
     0.5,  0.5, -0.5,    0.5,  0.5,  0.5,   0.5, -0.5,  0.5,

     0.5,  0.5, -0.5,   -0.5,  0.5, -0.5,  -0.5,  0.5,  0.5,
     0.5,  0.5, -0.5,   -0.5,  0.5,  0.5,   0.5,  0.5,  0.5,

     0.5, -0.5, -0.5,   -0.5, -0.5,  0.5,  -0.5, -0.5, -0.5,
     0.5, -0.5, -0.5,    0.5, -0.5,  0.5,  -0.5, -0.5,  0.5,
    };

    vector<float> normals = {
     0.0,  0.0,  1.0,    0.0,  0.0,  1.0,   0.0,  0.0,  1.0,
     0.0,  0.0,  1.0,    0.0,  0.0,  1.0,   0.0,  0.0,  1.0,

     0.0,  0.0, -1.0,    0.0,  0.0, -1.0,   0.0,  0.0, -1.0,
     0.0,  0.0, -1.0,    0.0,  0.0, -1.0,   0.0,  0.0, -1.0,

    -1.0,  0.0,  0.0,   -1.0,  0.0,  0.0,  -1.0,  0.0,  0.0,
    -1.0,  0.0,  0.0,   -1.0,  0.0,  0.0,  -1.0,  0.0,  0.0,

     1.0,  0.0,  0.0,    1.0,  0.0,  0.0,   1.0,  0.0,  0.0,
     1.0,  0.0,  0.0,    1.0,  0.0,  0.0,   1.0,  0.0,  0.0,

     0.0,  1.0,  0.0,    0.0,  1.0,  0.0,   0.0,  1.0,  0.0,
     0.0,  1.0,  0.0,    0.0,  1.0,  0.0,   0.0,  1.0,  0.0,

     0.0, -1.0,  0.0,    0.0, -1.0,  0.0,   0.0, -1.0,  0.0,
     0.0, -1.0,  0.0,    0.0, -1.0,  0.0,   0.0, -1.0,  0.0,

    };

    setVertices(vertices);
    setNormals(normals);

    setMode(GL_TRIANGLES);
}

Cube::~Cube() {
    cout << this << " [Cube] destructor" << endl;
}
