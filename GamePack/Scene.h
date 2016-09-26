#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>

//#include <btBulletDynamicsCommon.h>

// Includes GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Object.h"

#include "Camera.h"
#include "Light.h"

class Scene: public Object
{

	public:

		Scene(int width = 1920, int height = 1080);
		virtual ~Scene();

		void load();

		void step(int fps = 60);
		void render();

	private:

		int m__width;
		int m__height;

		bool m__paused;

		glm::mat4 m__projection;
		glm::mat4 m__view;
		glm::mat4 m__model;

		Camera* m__cam;

		/*btBroadphaseInterface* m__broadphase;
		btDefaultCollisionConfiguration* m__collisionConfiguration;
		btCollisionDispatcher* m__dispatcher;
		btSequentialImpulseConstraintSolver* m__solver;
		btDiscreteDynamicsWorld* m__dynamicsWorld;

		btCollisionShape* m__groundShape;
		btDefaultMotionState* m__groundMotionState;
		btRigidBody* m__groundRigidBody;*/

		//EnvironmentObject* m__environmentObject;
		//PostProcessObject* m__postProcess;
		std::vector<Light*> m__lights;

		void renderObjects();
		void renderLights();

};
