#pragma once

#include "SteeringBehavior.h"

namespace AI
{

	class Agent;

	class SeparationBehavior : public SteeringBehavior
	{
	public:
		X::Math::Vector2 Calculate(Agent& Agent);
	};

	class AlignmentBehavior : public SteeringBehavior
	{
	public:
		X::Math::Vector2 Calculate(Agent& Agent);
	};

	class CohesionBehavior : public SteeringBehavior
	{
	public:
		X::Math::Vector2 Calculate(Agent& Agent);
	};
}