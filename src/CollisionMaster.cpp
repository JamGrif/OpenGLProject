#include "CollisionMaster.h"

#include "CollisionObject.h"

#include <iostream>

std::vector<CollisionObject*> CollisionMaster::m_sceneCollisionObjects;

CollisionMaster::CollisionMaster()
{
	std::cout << "collisionMaster made" << std::endl;
}

CollisionMaster::~CollisionMaster()
{
	std::cout << "collisionMaster destroyed" << std::endl;
}

void CollisionMaster::addToCollisionObjects(CollisionObject* cObject)
{
	m_sceneCollisionObjects.push_back(cObject);
}
