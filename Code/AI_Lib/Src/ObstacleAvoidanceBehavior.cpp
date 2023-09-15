#include "Precompiled.h"
#include "ObstacleAvoidanceBehavior.h"
#include "Agent.h"
#include "AIWorld.h"

using namespace AI;
X::Math::Vector2 ObstacleAvoidanceBehavior::Calculate(Agent& agent)
{
	const auto destination = agent.destination;
	const auto distance = X::Math::Magnitude(destination - agent.position);

	X::Math::Vector2 targetPos = X::Math::Vector2::Zero();
	int numOfObstacles = 0;
	auto worldTrans = agent.GetWorldTransform();
	auto invTransform = X::Math::Inverse(worldTrans);
	auto& obstacles = agent.world.GetObstacles();
	for (auto& obstacle : obstacles)
	{
		auto localPos = X::Math::TransformCoord(obstacle.center, invTransform);
		if (X::Math::Magnitude(localPos) > distance || localPos.x <= 0.0f)
		{
			continue;
		}
		if (X::Math::Abs(localPos.y) - (obstacle.radius + mOffset) > agent.radius)
		{
			continue;
		}
		float overlap = (obstacle.radius + mOffset + agent.radius) - X::Math::Abs(localPos.y);
		X::Math::Vector2 localObstaclePos = { localPos.x - (obstacle.radius + mOffset + agent.radius), 0.0f };
		if (localObstaclePos.x < 0.0f)
		{
			localObstaclePos.x = localPos.x + (obstacle.radius + mOffset + agent.radius);
			targetPos += X::Math::TransformCoord(localObstaclePos, worldTrans);
		}
		localObstaclePos.y = (localPos.y > 0.0f) ? -overlap : overlap;
		targetPos += X::Math::TransformCoord(localObstaclePos, worldTrans);
		++numOfObstacles;
	}
	X::Math::Vector2 steeringForce = X::Math::Vector2::Zero();
	X::Math::Vector2 desiredVelocity = X::Math::Vector2::Zero();
	if (numOfObstacles > 0)
	{
		targetPos /= (float)numOfObstacles;
		const auto agentToDest = targetPos - agent.position;
		const float distToDest = X::Math::Magnitude(agentToDest);
		float speed = 0.0f;
		if (distToDest + 1.0f)
		{
			const float decelTweaker = agent.mass * 1.5f;
			speed = distToDest / decelTweaker;
			speed = X::Math::Min(speed, agent.maxSpeed);
		}

		desiredVelocity = (agentToDest / distToDest) * speed;
		steeringForce = desiredVelocity - agent.velocity;
	}

	if (IsDebug())
	{
		X::DrawScreenLine(agent.position, agent.position + desiredVelocity, X::Colors::CadetBlue);
		X::DrawScreenLine(agent.position, agent.position + agent.velocity, X::Colors::Aquamarine);
		X::DrawScreenLine(agent.position, targetPos, X::Colors::HotPink);

		X::Math::Vector2 pA = X::Math::TransformCoord({ 0.0f, -agent.radius }, worldTrans);
		X::Math::Vector2 pB = X::Math::TransformCoord({ 0.0f, agent.radius }, worldTrans);
		X::Math::Vector2 pC = X::Math::TransformCoord({ distance, -agent.radius }, worldTrans);
		X::Math::Vector2 pD = X::Math::TransformCoord({ distance, agent.radius }, worldTrans);

		X::DrawScreenLine(pA, pB, X::Colors::AliceBlue);
		X::DrawScreenLine(pA, pC, X::Colors::AliceBlue);
		X::DrawScreenLine(pB, pD, X::Colors::AliceBlue);
		X::DrawScreenLine(pC, pD, X::Colors::AliceBlue);
		X::DrawScreenLine(pA, pB, X::Colors::AliceBlue);

	}
	return steeringForce;
}



