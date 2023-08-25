#include "Peon.h"
#include "TypeIds.h"

extern float wanderJitter;
extern float wanderRadius;
extern float wanderDistance;

Peon::Peon(AI::AIWorld& world)
	: Agent(world, Types::PeonId)
{
	
}

void Peon::Load()
{
	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);
	mWanderBehavior = mSteeringModule->AddBehavior<AI::WanderBehavior>();
	mArriveBehavior = mSteeringModule->AddBehavior<AI::ArriveBehavior>();
	mEvadeBehavior = mSteeringModule->AddBehavior<AI::EvadeBehavior>();
	mPursuitBehavior = mSteeringModule->AddBehavior<AI::PursuitBehavior>();
	mFleeBehavior = mSteeringModule->AddBehavior<AI::FleeBehavior>();
	mSeekBehavior = mSteeringModule->AddBehavior<AI::SeekBehavior>();

	mWanderBehavior->SetActive(true);

	for (size_t i = 0; i < mTextureIds.size(); ++i)
	{
		char name[128];
		sprintf_s(name, "scv_%02i.png", i+1);
		mTextureIds[i] = X::LoadTexture(name);
	}
}

void Peon::Unload()
{

}

void Peon::Update(float deltaTime)
{
	if (mWanderBehavior->GetActive())
	{
		mWanderBehavior->Setup(wanderRadius, wanderDistance, wanderJitter);
	}
	const auto force = mSteeringModule->Calculate();
	const auto acceleration = force / mass;
	velocity += acceleration * deltaTime;
	if (X::Math::MagnitudeSqr(velocity) > 1.0f)
	{
		heading = X::Math::Normalize(velocity);
	}

	position += velocity * deltaTime;

	const auto screenWidth = X::GetScreenWidth();
	const auto screenHeight = X::GetScreenHeight();

	if (position.x < 0.0f)
	{
		position.x += screenWidth;
	}
	if (position.x >= screenWidth)
	{
		position.x -= screenWidth;
	}
	if (position.y < 0.0f)
	{
		position.y += screenHeight;
	}
	if (position.y >= screenHeight)
	{
		position.y -= screenHeight;
	}
}

void Peon::Render()
{
	const float angle = atan2(-heading.x, heading.y) + X::Math::kPi;
	const float percent = angle / X::Math::kTwoPi;
	const int frame = static_cast<int>(percent * mTextureIds.size()) % mTextureIds.size();
	X::DrawSprite(mTextureIds[frame], position);
}

void Peon::ShowDebug(bool debug)
{
	mWanderBehavior->ShowDebug(debug);
	mEvadeBehavior->ShowDebug(debug);
	mPursuitBehavior->ShowDebug(debug);
	mFleeBehavior->ShowDebug(debug);
	mSeekBehavior->ShowDebug(debug);
	mWanderBehavior->ShowDebug(debug);
}
