#include "MeshRender.h"

#include <iostream>

using std::cout;
using std::endl;
using std::vector;
using glm::vec2;
using glm::vec3;
using glm::mat4;

using namespace LORE;

MeshRender::MeshRender():	Mesh(),
							m__VAO(),
							m__VBO(),
							m__UVs(),
							m__normals() {
	cout << this << " [MeshRender] constructor" << endl;
}


MeshRender::MeshRender(const MeshRender*) : Mesh(),
												m__VAO(),
												m__VBO(),
												m__UVs(),
												m__normals() {
	cout << this << " [MeshRender] copy-constructor" << endl;

	m__UVs.push_back(vec2(1, 0));
	m__UVs.push_back(vec2(0, 1));
	m__UVs.push_back(vec2(1, 0));

	m__normals.push_back(vec3(-1, 0, 0));
	m__normals.push_back(vec3(0, 1, 0));
	m__normals.push_back(vec3(1, 0, 0));
}

MeshRender::~MeshRender() {
	cout << this << " [MeshRender] destructor" << endl;
}

void MeshRender::computeTangentBasis(   // inputs
	vector<vec3> & vertices,
	vector<vec2> & uvs,
	vector<vec3> & normals,

	// outputs
	vector<vec3> & tangents,
	vector<vec3> & bitangents) {

	for (unsigned int i = 0; i<vertices.size(); i += 3) {
		// Shortcuts for vertices
		vec3 & v0 = vertices[i + 0];
		vec3 & v1 = vertices[i + 1];
		vec3 & v2 = vertices[i + 2];

		// Shortcuts for UVs
		vec2 & uv0 = uvs[i + 0];
		vec2 & uv1 = uvs[i + 1];
		vec2 & uv2 = uvs[i + 2];

		// Edges of the triangle : postion delta
		vec3 deltaPos1 = v1 - v0;
		vec3 deltaPos2 = v2 - v0;

		// UV delta
		vec2 deltaUV1 = uv1 - uv0;
		vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
		vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

		// Set the same tangent for all three vertices of the triangle.

		// They will be merged later, in vboindexer.cpp
		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		// Same thing for binormals
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
	}
}

void MeshRender::load() {
	Mesh::load();

	cout << this << " [MeshRender] load" << endl;

	int sizeVerticesBytes = this->getVerticesSize();
	int sizeUVsBytes = this->getUVsSize();
	int sizeNormalsBytes = this->getNormalsSize();

	float* vertices = this->getVerticesFloat();
	float* UVs = this->getUVsFloat();
	float* normals = this->getNormalsFloat();

	//Tangent compute
	vector<vec3> vecTangents = this->getNormals();
	vector<vec3> vecBytangents = this->getNormals();

	//computeTangentBasis(this->getVertices(), this->getUVs(), this->getNormals(), vecTangents, vecBytangents);

	float* tangents = this->vec3ToFloat(vecTangents);
	float* bytangents = this->vec3ToFloat(vecBytangents);

	GLuint& idVAO = this->getVAO();
	GLuint& idVBO = this->getVBO();

	//===============================VBO================================

	// Destruction d'un �ventuel ancien VBO
	if (glIsBuffer(idVBO) == GL_TRUE)
		glDeleteBuffers(1, &idVBO);

	// G�n�ration de l'ID
	glGenBuffers(1, &idVBO);

	// Verrouillage du VBO
	glBindBuffer(GL_ARRAY_BUFFER, idVBO);

		// Allocation de la m�moire vid�o
		glBufferData(GL_ARRAY_BUFFER, sizeVerticesBytes + sizeUVsBytes + sizeNormalsBytes*3, 0, GL_STATIC_DRAW); // * 3 for Tangent and Bytangent

		// Transfert des donn�es
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeVerticesBytes, vertices);
		glBufferSubData(GL_ARRAY_BUFFER, sizeVerticesBytes,  sizeUVsBytes,  UVs);

		glBufferSubData(GL_ARRAY_BUFFER, sizeVerticesBytes + sizeUVsBytes,  sizeNormalsBytes, normals);
		glBufferSubData(GL_ARRAY_BUFFER, sizeVerticesBytes + sizeUVsBytes + sizeNormalsBytes, sizeNormalsBytes, tangents);
		glBufferSubData(GL_ARRAY_BUFFER, sizeVerticesBytes + sizeUVsBytes + sizeNormalsBytes * 2, sizeNormalsBytes, bytangents);

	// D�verrouillage de l'objet
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//=======================================================================
	// Destruction d'un �ventuel ancien VAO

	if (glIsVertexArray(idVAO) == GL_TRUE)
		glDeleteVertexArrays(1, &idVAO);

	// G�n�ration de l'identifiant du VAO
	glGenVertexArrays(1, &idVAO);

	// Verrouillage du VAO
	glBindVertexArray(idVAO);

		// Verrouillage du VBO
		glBindBuffer(GL_ARRAY_BUFFER, idVBO);

			// Acc�s aux vertices dans la m�moire vid�o

			//Vertices
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
			glEnableVertexAttribArray(0);

			//UVs
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeVerticesBytes));
			glEnableVertexAttribArray(1);

			//Normals
			/*glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeVerticesBytes + sizeUVsBytes));
			glEnableVertexAttribArray(2);

			//Tangents
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeVerticesBytes + sizeUVsBytes + sizeNormalsBytes));
			glEnableVertexAttribArray(3);

			//Bytangents
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeVerticesBytes + sizeUVsBytes + sizeNormalsBytes + sizeNormalsBytes)); //sizeTangentBytes
			glEnableVertexAttribArray(4);*/

		// D�verrouillage du VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	// D�verrouillage du VAO
	glBindVertexArray(0);

	/*
	cout << "[MeshRender] load() : Vertices : \n";
	vector<vec3> vecVertices = this->getVertices();
	for (int i = 0; i < this->getVertexCount(); i++)
	{

		cout << vecVertices[i].x << ", " << vecVertices[i].y << ", " << vecVertices[i].z << endl;

	}

	cout << "[MeshRender] load() : UVs : \n";
	for (int i = 0; i < this->getUVCount(); i++)
	{

		cout << m__UVs[i].x << ", " << m__UVs[i].y << endl;

	}

	cout << "[MeshRender] load() : Normals : \n";
	for (int i = 0; i < this->getNormalCount(); i++)
	{

		cout << m__normals[i].x << ", " << m__normals[i].y << ", " << m__normals[i].z << endl;

	}
	*/

	free(vertices);
	free(UVs);
	free(normals);
}

void MeshRender::render(mat4 &projection, mat4 &view, mat4 &model, GLuint environmentMapID) {
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	Shader* s = getMaterial()->getShader();

	glUseProgram(s->getProgramID());

	glBindVertexArray(getVAO());

	s->envoyerMat4("projection", projection);
	s->envoyerMat4("view", view);
	s->envoyerMat4("model", model);
	s->envoyerMat4("MVP", projection * view * model);

	/*s->envoyerVec3("cameraPos", cameraPos);
	s->envoyerMat4("cameraProjection", depthProjection);
	s->envoyerMat4("cameraView", depthView);
	s->envoyerMat4("cameraModel", depthModel);
	s->envoyerMat4("cameraMVP", depthBias * (depthProjection * (depthView * depthModel)));*/

	/*s->envoyerMat3("MV3x3", mat3(view * model));
	s->envoyerMat3("M3x3", mat3(model));*/

	/*s->envoyerVec3("lightPositionWorldspace", Light::getInstance()->getLightPos());*/

	s->envoyerVec3("colorDiffuse", getMaterial()->getDiffuseColor());

	/*s->envoyerFloat("utilisationDiffuse", m__material->getUtilisationDiffuse());
	s->envoyerFloat("utilisationNormal", m__material->getUtilisationNormal());
	s->envoyerFloat("utilisationSpecular", m__material->getUtilisationSpecular());
	s->envoyerFloat("utilisationDisplacement", m__material->getUtilisationDisplacement());
	s->envoyerFloat("utilisationReflection", m__data->getMaterial()->getUtilisationReflection());
	s->envoyerFloat("utilisationRefraction", m__data->getMaterial()->getUtilisationRefraction());*/

	s->envoyer1I("shadowMap1", 0);
	s->envoyer1I("shadowMap2", 0);
	s->envoyer1I("environment", 1);
	s->envoyer1I("diffuse", 2);
	s->envoyer1I("normal", 3);
	s->envoyer1I("specular", 4);
	s->envoyer1I("displacement", 5);
	s->envoyer1I("reflection", 6);
	s->envoyer1I("refraction", 7);

	//s->envoyerFloat("spreadFactor", Light::getInstance()->spread);

	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowMap);*/

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, environmentMapID);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, getMaterial()->getDiffuseTexture()->getID());

/*
	if (m__data->getMaterial()->getUtilisationNormal())
	{

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m__data->getMaterial()->getTextureNormal()->getTextureID());

	}

	if (m__data->getMaterial()->getUtilisationSpecular())
	{

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m__data->getMaterial()->getTextureSpecular()->getTextureID());

	}

	if (m__data->getMaterial()->getUtilisationDisplacement())
	{

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m__data->getMaterial()->getTextureDisplacement()->getTextureID());

	}

	if (m__data->getMaterial()->getUtilisationReflection())
	{

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m__data->getMaterial()->getTextureReflection()->getTextureID());

	}

	if (m__data->getMaterial()->getUtilisationRefraction())
	{

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, m__data->getMaterial()->getTextureRefraction()->getTextureID());

	}*/

	/*glPatchParameteri(GL_PATCH_VERTICES, 3);
	glDrawArrays(GL_PATCHES, 0, getVertexCount());*/

	glDrawArrays(GL_TRIANGLES, 0, getVertexCount());

	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, 0);*/

	glBindVertexArray(0);

	glUseProgram(0);


	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((const GLfloat*)&projection[0]);

	glMatrixMode(GL_MODELVIEW);
	mat4 MV = view * model;
	glLoadMatrixf((const GLfloat*)&MV[0]);


	glColor3f(0,0,1);

	glBegin(GL_LINES);

	vector<vec3> vertices = this->getVertices();

	for (unsigned int i=0; i < vertices.size()-1; i++){
		vec3 p = vertices[i];
		glVertex3fv(&p.x);

		vec3 o = normalize(m__normals[i]);
		p+=o*0.1f;
		glVertex3fv(&p.x);
	}
	glEnd();
}

//=====VAO/VBO=====
GLuint& MeshRender::getVAO() {
	return m__VAO;
}

GLuint& MeshRender::getVBO() {
	return m__VBO;
}

//=====UVs=====
int MeshRender::getUVCount() {
	return m__UVs.size();
}

int MeshRender::getUVsSize() {
	return m__UVs.size() * 2 * sizeof(float);
}

vector<vec2> MeshRender::getUVs() {
	return m__UVs;
}

float* MeshRender::getUVsFloat() {
	float* res = (float*)malloc(sizeof(float) * m__UVs.size() * 2);
	for (unsigned int i = 0; i < m__UVs.size(); i++) {
		res[i*2] = m__UVs[i].x;
		res[i*2 + 1] = m__UVs[i].y;
	}
	return res;
}

//=====Normals=====
int MeshRender::getNormalCount() {
	return m__normals.size();
}

int MeshRender::getNormalsSize() {
	return m__normals.size() * 3 * sizeof(float);
}

vector<vec3> MeshRender::getNormals() {
	return m__normals;
}

float* MeshRender::getNormalsFloat() {
	float* res = (float*)malloc(sizeof(float) * m__normals.size() * 3);
	for (unsigned int i = 0; i < m__normals.size(); i += 3) {
		res[i] = m__normals[i].x;
		res[i + 1] = m__normals[i].y;
		res[i + 2] = m__normals[i].z;
	}
	return res;
}
