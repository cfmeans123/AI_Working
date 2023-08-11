#include "Precompiled.h"
#include "SteeringModule.h"

using namespace AI;

SteeringModule::SteeringModule(Agent& agent)
	: mAgent(agent)
{

}

X::Math::Vector2 SteeringModule::Calculate()
{
	X::Math::Vector2 totalForce;
	for (auto& behavior : mBehaviors)
	{
		totalForce += behavior->Calculate(mAgent) * behavior->GetWeight();
	}
	return totalForce;

}