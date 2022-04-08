#include "pch.h"
#include "CollisionObject.h"

#include <assimp/Importer.hpp>		// Mesh loading
#include <assimp/scene.h>			//
#include <assimp/postprocess.h>		//

#include "CollisionMaster.h"

CollisionObject::CollisionObject()
{
	//std::cout << "collisionObject made" << std::endl;


	CollisionMaster::addToCollisionObjects(this);
}

CollisionObject::~CollisionObject()
{
	//std::cout << "collisionObject destroyed" << std::endl;
}

void CollisionObject::init(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, const std::string& filepath)
{
	
	m_shape = new btBoxShape(btVector3(btScalar(1.1f), btScalar(1.1f), btScalar(1.1f)));

	btTransform m_transform;
	m_transform.setIdentity();
	m_transform.setOrigin(btVector3(position.x, position.y, position.z));
	m_transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));

	myMotionState = new btDefaultMotionState(m_transform);

	btVector3 localInertia(0, 0, 0);
	float m_mass;

	if (m_gravity)
	{
		m_mass = 1.0f;
		m_shape->calculateLocalInertia(m_mass, localInertia);
	}
	else
	{
		m_mass = 0.0f;
	}

	btRigidBody::btRigidBodyConstructionInfo bInfo(m_mass, myMotionState, m_shape, localInertia);

	m_body = new btRigidBody(bInfo);
	//m_body->setRestitution(0.2);
	//m_body->setFriction(0.9);
	//m_body->setUserIndex(0);
	//m_body->setActivationState(DISABLE_DEACTIVATION);
	//m_body->setCollisionFlags(m_body->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
	//m_body->setUserPointer(m_body);

	CollisionMaster::getDynamicsWorld()->addRigidBody(m_body);

}

void CollisionObject::update(glm::vec3& position, glm::vec3& scale, glm::vec3& rotation)
{
	btTransform m_transform;
	myMotionState->getWorldTransform(m_transform);

	
	float x = float(m_transform.getOrigin().getX());
	float y = float(m_transform.getOrigin().getY());
	float z = float(m_transform.getOrigin().getZ());
	position.x = x;
	position.y = y;
	position.z = z;

	//btScalar myscaler;
	//btScalar myscaler2;
	//btScalar myscaler3;
	//
	//m_transform.getRotation().getEulerZYX(myscaler, myscaler2, myscaler3);
	//float x2 = float(myscaler);
	//float y2 = float(myscaler2);
	//float z2 = float(myscaler3);
	//rotation.x = x2;
	//rotation.y = y2;
	//rotation.z = z2;



	//if (m_gravity)
	//{
	//	std::cout << "gravity ON" << std::endl;
	//}
	//else
	//{
	//	std::cout << "gravity OFF" << std::endl;
	//}
}

void CollisionObject::affectedByGravity(bool gravity)
{
	m_gravity = gravity;
}

btCollisionShape* CollisionObject::getBtCollisionShape()
{
	return m_shape;
}

void CollisionObject::setXYZ(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}
