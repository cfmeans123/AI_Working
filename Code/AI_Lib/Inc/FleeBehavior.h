#pragma once
#include "SteeringBehavior.h"

namespace AI
{
	class FleeBehavior : public SteeringBehavior
	{
	public:
		X::Math::Vector2 Calculate(Agent& agent) override;

		float fleeDistance = 100.0f;
	};
}