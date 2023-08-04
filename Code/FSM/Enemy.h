#pragma once
#include <AI.h>
#include "EnemyStates.h"


enum EnemyStates
{
	Idle,
	Patrol,
	Engage,
	Recover,
	Destroy
};

class Enemy
{
public:
	enum LocationState
	{
		Streets,
		Alleys,
		Residential,
		Tenderloin
	};
	Enemy() = default;
	void Initialize();
	void Terminate();
	void Update(float dt);
	void ChangeState(EnemyStates newState);
	void SetLocation(LocationState loc);
	bool IsActive() { return mActive; }
	bool IsPatrolComplete() const;
	bool Rested() const;
	bool PatrolHasStarted();
	bool PatrolHasComplete();


	float IsAlive() const;
	float GetHealth() { return mHealth; }

	void IncreaseFatigue();
	void StartTimer();


private:
	AI::StateMachine<Enemy>* mStateMachine;
	LocationState mLocation;
	bool mActive = false;
	bool mEnemyFound = false;
	float mHealth;
	float mFatigue;
	//Stopwatch<float>* mStopWatch;
};



