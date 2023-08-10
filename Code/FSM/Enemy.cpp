#include "Enemy.h"
#include "EnemyStates.h"

void Enemy::Initialize()
{
	mLocation = Enemy::LocationState::Alleys;
	mActive = true;
	mEnemyFound = false;
	mHealth = 100.0f;
	mHealthCap = 100.0f;
	mFatigue = 0.0f;
	mDamageMax = 25;
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
void Enemy::TakeDamage(int dam)
{
	mHealth -= dam;
}

void Enemy::ResetHealth()
{
	mHealth = mHealthCap;
}
void Enemy::ResetFatigue()
{
	mFatigue = 0.0f;
}

bool Enemy::IsAlive() const
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

void Enemy::IncreaseFatigue(float val)
{
	mFatigue += val;
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
	if (myTimer.GetActiveState())
	{
		ImGui::Text("Current timer holds: [%f]", myTimer.mTime);
	}
}

void Enemy::StartTimer()
{
}

void Enemy::IterateTimer(float dt)
{
}