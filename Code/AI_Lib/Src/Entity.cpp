#include "Precompiled.h"
#include "Entity.h"
#include "AIWorld.h"

using namespace AI;

Entity::Entity(AIWorld& world, uint32_t typeID)
	: world(world)
	, mUniqueId(static_cast<uint64_t>(typeID) << 32 | world.GetNextId())
{
	world.Register(this);
}
Entity::~Entity()
{
	world.Unregister(this);
}