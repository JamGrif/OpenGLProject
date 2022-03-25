#pragma once

#include <vector>

class CollisionObject;


class CollisionMaster
{
public:
	CollisionMaster();
	~CollisionMaster();

	static void addToCollisionObjects(CollisionObject* cObject);
	

private:

	static std::vector<CollisionObject*> m_sceneCollisionObjects;

};

