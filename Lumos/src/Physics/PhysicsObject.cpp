#include "lmpch.h"
#include "PhysicsObject.h"

namespace Lumos
{

	PhysicsObject::PhysicsObject() : m_Static(false), m_Elasticity(0.9f), m_Friction(0.8f), m_AtRest(false)
	{
	}

	PhysicsObject::~PhysicsObject() {}
}
