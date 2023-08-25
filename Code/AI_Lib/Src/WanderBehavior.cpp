#include "Precompiled.h"
#include "WanderBehavior.h"
#include "Agent.h"

using namespace AI;

X::Math::Vector2 WanderBehavior::Calculate(Agent& agent)
{
	//Apply random jitter to wander target
	auto newWanderTarget = mLocalWanderTarget + (X::RandomUnitCircle() * mWanderJitter);

	//snap the new position back to the wander circle
	newWanderTarget = X::Math::Normalize(newWanderTarget) * mWanderRadius;
	mLocalWanderTarget = newWanderTarget;

	//project target in front of agent
	newWanderTarget += X::Math::Vector2(0.0f, mWanderDistance);

	//transform the target into world space
	const auto worldTransform = agent.GetWorldTransform();
	const auto worldWanderTarget = X::Math::TransformCoord(newWanderTarget, worldTransform);

	//check if we are at the target
	const auto agentDist = worldWanderTarget - agent.position;
	const float distToDest = X::Math::Magnitude(agentDist);
	if (distToDest <= 0.01f)
	{
		return X::Math::Vector2::Zero();
	}

	//seek to wander target;
	const auto desiredVelocity = (agentDist / distToDest) * agent.maxSpeed;
	const auto seekForce = desiredVelocity - agent.velocity;

	if (IsDebug())
	{
		const auto wanderCircle = X::Math::TransformCoord({ 0.0f, mWanderDistance }, worldTransform);
		X::DrawScreenCircle(wanderCircle, mWanderRadius, X::Colors::AntiqueWhite);
		X::DrawScreenDiamond(worldWanderTarget, 3.0f, X::Colors::Azure);
		X::DrawScreenLine(agent.position, worldWanderTarget, X::Colors::Aqua);
	}

	return seekForce;
}

void WanderBehavior::Setup(float radius, float distance, float jitter)
{
	mWanderRadius = radius;
	mWanderDistance = distance;
	mWanderJitter = jitter;
}