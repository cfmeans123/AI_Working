#pragma once

#include "Entity.h"

namespace AI
{
	class AIWorld
	{
	public:
		void Initialize();
		void Update();

		void Register(Entity* entity);
		void Unregister(Entity* entity);

		EntityPtrs GetEntities() const { return mEntities; }
		EntityPtrs GetEntitiesInRange(const X::Math::Circle& range, uint32_t typeId);

		uint32_t GetNextId() const
		{
			XASSERT(mNextId < UINT32_MAX, "AIWorld: run out of IDs!");
			return mNextId++;
		}

	private:
		mutable uint32_t mNextId = 0;
		EntityPtrs mEntities;
	};
}