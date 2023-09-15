#pragma once
#include "SteeringBehavior.h"

namespace AI
{
	class ObstacleAvoidanceBehavior : public SteeringBehavior
	{
	public:
		X::Math::Vector2 Calculate(Agent& agent) override;

		float fleeDistance = 100.0f;
		float mOffset = 50.0f;
	};
}