#include "pch.h"
#include "CollisionMaster.h"

#include "CollisionObject.h"

std::vector<CollisionObject*> CollisionMaster::m_sceneCollisionObjects;

btDynamicsWorld* CollisionMaster::dynamicsWorld = nullptr;

CollisionMaster::CollisionMaster()
{
	//std::cout << "collisionMaster made" << std::endl;
	PRINT_INFO("collisionMaster made");

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a different dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broad phase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));




}

CollisionMaster::~CollisionMaster()
{
	//std::cout << "collisionMaster destroyed" << std::endl;
	PRINT_INFO("collisionMaster destroyed");

	delete dynamicsWorld;

	delete solver;

	delete overlappingPairCache;

	delete dispatcher;

	delete collisionConfiguration;
}

void CollisionMaster::addToCollisionObjects(CollisionObject* cObject)
{
	m_sceneCollisionObjects.push_back(cObject);
}

btDynamicsWorld* CollisionMaster::getDynamicsWorld()
{
	return dynamicsWorld;
}

void CollisionMaster::update()
{
	dynamicsWorld->stepSimulation(1.f / 60.f, 10);

	//for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	//{
	//	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
	//	btRigidBody* body = btRigidBody::upcast(obj);
	//	btTransform trans;
	//
	//	if (body && body->getMotionState())
	//	{
	//		std::cout << "hi" << std::endl;
	//		body->getMotionState()->getWorldTransform(trans);
	//	}
	//	else
	//	{
	//		std::cout << "second hi" << std::endl;
	//		trans = obj->getWorldTransform();
	//	}
	//	m_sceneCollisionObjects.at(j)->setXYZ(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	//	std::cout << "world pos " << j << " " << float(trans.getOrigin().getX()) << " " << float(trans.getOrigin().getY()) << " " << float(trans.getOrigin().getZ()) << std::endl;
	//}
}
