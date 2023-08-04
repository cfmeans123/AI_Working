#include "Enemy.h"

void Enemy::Initialize()
{
	mLocation = Enemy::LocationState::Alleys;
	mActive = false;
	mEnemyFound = false;
	mHealth = 100.0f;
	mFatigue = 0.0f;
	mStateMachine = new AI::StateMachine<Enemy>(*this);

	mStateMachine->AddState<IdleState>();
	mStateMachine->AddState<PatrolState>();
	mStateMachine->AddState<EngageState>();
	mStateMachine->AddState<RecoverState>();
	mStateMachine->AddState<DestroyState>();

}


void Enemy::Terminate()
{
	delete mStateMachine;
	mStateMachine = nullptr;
}
void Enemy::Update(float dt)
{
	mStateMachine->Update(dt);
}
void Enemy::ChangeState(EnemyStates newState)
{
	mStateMachine->ChangeState((int)newState);
}
void Enemy::SetLocation(Enemy::LocationState loc)
{
	mLocation = loc;
}
float Enemy::IsAlive() const
{
	return mHealth > 0.0f;
}
bool Enemy::IsPatrolComplete() const
{
	/*if (mStopWatch->tick() <= std::chrono::milliseconds(0))
	{
		return true;
	}*/
	return false;
}
bool Enemy::Rested() const
{
	return mFatigue > 0.0f;
}
bool Enemy::PatrolHasStarted()
{
	return 0;
	//return mStopWatch->tick().count() > 0.0f;
}
bool Enemy::PatrolHasComplete()
{
	return 0;
	//return mStopWatch->tick().count() <= 0.0f;
}

void Enemy::IncreaseFatigue()
{
	mFatigue += 1.0f;
}

void Enemy::StartTimer()
{
	//*mStopWatch = 3.0f;
}