#include "Precompiled.h"
#include "EvadeBehavior.h"
#include "Agent.h"

using namespace AI;


X::Math::Vector2 EvadeBehavior::Calculate(Agent& agent)
{
	X::Math::Vector2 desiredVelocity = X::Math::Vector2::Zero();
	const auto agentToDest = agent.target->position - agent.position;
	const float distToDest = X::Math::Magnitude(agentToDest);

	if (distToDest < evadeDistance)
	{
		desiredVelocity = -(agentToDest / distToDest) * agent.maxSpeed;
	}

	X::Math::Vector2 fleeForce = desiredVelocity - agent.velocity;

	if (IsDebug())
	{
		X::DrawScreenLine(agent.position, agent.position + desiredVelocity, X::Colors::Yellow);
		X::DrawScreenLine(agent.position, agent.position + agent.velocity, X::Colors::Green);
		X::DrawScreenCircle(agent.target->position, 20.0f, X::Colors::Red);
	}

	return fleeForce;
}