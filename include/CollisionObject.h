#pragma once

#include "btBulletDynamicsCommon.h"

#include <glm\glm.hpp>
//#include <glm\gtc\type_ptr.hpp>
//#include <glm\gtc\matrix_transform.hpp>
//#include <glm\gtx\transform.hpp>

class CollisionObject
{
public:
	CollisionObject();
	~CollisionObject();

	void init(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, const std::string& filepath);

	void update(glm::vec3& position, glm::vec3& scale, glm::vec3& rotation);

	void affectedByGravity(bool gravity);

	btCollisionShape* getBtCollisionShape();

	void setXYZ(float x, float y, float z);


private:

	btCollisionShape* m_shape = nullptr;
	
	btDefaultMotionState* myMotionState = nullptr;
	btRigidBody* m_body = nullptr;

	float m_x, m_y, m_z;
	bool m_gravity = false;

};

