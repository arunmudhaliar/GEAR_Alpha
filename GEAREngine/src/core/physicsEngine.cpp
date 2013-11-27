#include "physicsEngine.h"
#include "gxMesh.h"

physicsEngine::physicsEngine()
{
	m_broadphase = NULL;
	m_dispatcher = NULL;
	m_solver = NULL;
	m_collisionConfiguration = NULL;
	m_dynamicsWorld = NULL;
}

physicsEngine::~physicsEngine()
{
	exitPhysics();
}

void physicsEngine::clientResetScene()
{
	exitPhysics();
	initPhysics();
}

void physicsEngine::initPhysics()
{
	///collision configuration contains default setup for memory, collision setup
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	//m_collisionConfiguration->setConvexConvexMultipointIterations();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);

	m_broadphase = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	m_solver = new btSequentialImpulseConstraintSolver;

	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
	//m_dynamicsWorld->setDebugDrawer(&gDebugDraw);
	
	m_dynamicsWorld->setGravity(btVector3(0, 0, -10));

}

void physicsEngine::exitPhysics()
{
	//cleanup in the reverse order of creation/initialization

	//remove the rigidbodies from the dynamics world and delete them
	int i;
	for (i=m_dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
	{
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}

	//delete collision shapes
	for (int j=0;j<m_collisionShapes.size();j++)
	{
		btCollisionShape* shape = m_collisionShapes[j];
		delete shape;
	}
	m_collisionShapes.clear();

	delete m_dynamicsWorld;
	delete m_solver;
	delete m_broadphase;
	delete m_dispatcher;
	delete m_collisionConfiguration;
}

void physicsEngine::update(float dt)
{
	m_dynamicsWorld->stepSimulation(dt);
}
//http://www.bulletphysics.org/Bullet/phpBB3/viewtopic.php?f=9&t=2961
void physicsEngine::addRigidBody(object3d* obj)
{
	vector3f sz((obj->getOOBB().m_max - obj->getOOBB().m_min)*0.5f);

	btBoxShape* colShape = new btBoxShape(btVector3(sz.x, sz.y, sz.z));
	//btCollisionShape* colShape = new btSphereShape(btScalar(1.));
	m_collisionShapes.push_back(colShape);

	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar	mass(1.f);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	vector3f pos(obj->getWorldMatrix()->getPosition());

	btVector3 localInertia(pos.x, pos.y, pos.z);
	if (isDynamic)
		colShape->calculateLocalInertia(mass,localInertia);

	startTransform.setFromOpenGLMatrix(obj->getWorldMatrix()->getOGLMatrix());

	//vector3f center_oobb(obj->getOOBB().m_min+sz);
	//vector3f offset_center(obj->getWorldMatrix()->getPosition()-center_oobb);
	//btTransform centeroffTransform;
	//centeroffTransform.setIdentity();
	//centeroffTransform.setOrigin(btVector3(offset_center.x, offset_center.y, offset_center.z));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	
	obj->setRigidBody(body);
	m_dynamicsWorld->addRigidBody(body);
}

void physicsEngine::addBoxCollider(object3d* obj)
{
	vector3f sz((obj->getOOBB().m_max - obj->getOOBB().m_min)*0.5f);

	btBoxShape* colShape = new btBoxShape(btVector3(sz.x, sz.y, sz.z));
	//btCollisionShape* colShape = new btSphereShape(btScalar(1.));
	m_collisionShapes.push_back(colShape);

	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar	mass(0.f);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	vector3f pos(obj->getWorldMatrix()->getPosition());

	btVector3 localInertia(pos.x, pos.y, pos.z);
	if (isDynamic)
		colShape->calculateLocalInertia(mass,localInertia);

	startTransform.setFromOpenGLMatrix(obj->getWorldMatrix()->getOGLMatrix());
	//startTransform.setOrigin(btVector3(0, 0, 0));

	//vector3f center_oobb(obj->getOOBB().m_min+sz);
	//vector3f offset_center(obj->getWorldMatrix()->getPosition()-center_oobb);
	//btTransform centeroffTransform;
	//centeroffTransform.setIdentity();
	//centeroffTransform.setOrigin(btVector3(offset_center.x, offset_center.y, offset_center.z));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	
	obj->setRigidBody(body);
	m_dynamicsWorld->addRigidBody(body);
}