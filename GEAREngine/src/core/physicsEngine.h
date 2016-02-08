#ifdef USE_BULLET
#pragma once

///btBulletDynamicsCommon.h is the main Bullet include file, contains most common include files.
#include "../physics/btBulletDynamicsCommon.h"
#include "../physics/debug/GLDebugDrawer.h"
#include <vector>

#include "object3d.h"

class DECLSPEC physicsEngine
{
public:
	physicsEngine();
	~physicsEngine();

	void clientResetScene();
	void initPhysics();
	void exitPhysics();

	void update(float dt);
	void render();

	void addRigidBody(object3d* obj);
	void addBoxCollider(object3d* obj);

private:
	//keep the collision shapes, for deletion/cleanup
	std::vector<btCollisionShape*>	m_collisionShapes;
	btBroadphaseInterface*	m_broadphase;
	btCollisionDispatcher*	m_dispatcher;
	btConstraintSolver*	m_solver;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btDiscreteDynamicsWorld* m_dynamicsWorld;
	GLDebugDrawer m_cDebugDraw;
};

#endif	//#ifdef USE_BULLET