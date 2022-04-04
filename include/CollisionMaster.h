#pragma once

#include "btBulletDynamicsCommon.h"

class CollisionObject;


class CollisionMaster
{
public:
	CollisionMaster();
	~CollisionMaster();

	static void addToCollisionObjects(CollisionObject* cObject);

	static btDynamicsWorld* getDynamicsWorld();

	static void update();
	

private:

	static std::vector<CollisionObject*> m_sceneCollisionObjects;



	btDefaultCollisionConfiguration* collisionConfiguration;

	btCollisionDispatcher* dispatcher;

	btBroadphaseInterface* overlappingPairCache;

	btSequentialImpulseConstraintSolver* solver;

	static btDynamicsWorld* dynamicsWorld;

};

