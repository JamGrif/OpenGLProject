#include "CollisionObject.h"

#include <iostream>

#include "CollisionMaster.h"

CollisionObject::CollisionObject()
{
	std::cout << "collisionObject made" << std::endl;


	CollisionMaster::addToCollisionObjects(this);
}

CollisionObject::~CollisionObject()
{
	std::cout << "collisionObject destroyed" << std::endl;
}

void CollisionObject::update()
{

}
