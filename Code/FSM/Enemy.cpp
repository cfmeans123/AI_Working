#include "Enemy.h"
#include "EnemyStates.h"

void Enemy::Initialize()
{
	mLocation = Enemy::LocationState::Alleys;
	mActive = true;
	mEnemyFound = false;
	mHealth = 100.0f;
	mFatigue = 0.0f;
	mStateMachine = new AI::StateMachine<Enemy>(*this);

	mStateMachine->AddState<IdleState>();
	mStateMachine->AddState<PatrolState>();
	mStateMachine->AddState<EngageState>();
	mStateMachine->AddState<RecoverState>();
	mStateMachine->AddState<DestroyState>();

	mStateMachine->ChangeState(Idle);

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
void Enemy::ChangeState(EnemyState newState)
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

	return false;
}
bool Enemy::Rested() const
{
	return mFatigue > 0.0f;
}
bool Enemy::PatrolHasStarted()
{
	return false;
}
bool Enemy::PatrolHasComplete()
{
	return false;
}

void Enemy::IncreaseFatigue()
{
	mFatigue += 1.0f;
}

void Enemy::StartTimer()
{
	
	
}

void Enemy::IterateTimer(float dt)
{
	
}

void Enemy::ChangeActiveState()
{
	mActive = !mActive;
	myTimer.ResetTimer(10.0f);
}

void Enemy::DebugUI()
{
	mStateMachine->DebugUI();
	ImGui::Text("Your current fatigue is [%f]", mFatigue);
	ImGui::Text("Current timer holds: [%f]", myTimer.mTime);
}