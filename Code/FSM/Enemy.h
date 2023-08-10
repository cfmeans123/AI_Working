#pragma once
#include <AI.h>
#include "Stopwatch.h"


enum EnemyState
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
	void ChangeState(EnemyState newState);
	void SetLocation(LocationState loc);
	bool IsActive() { return mActive; }
	bool IsPatrolComplete() const;
	bool Rested() const;
	bool PatrolHasStarted();
	bool PatrolHasComplete();


	bool IsAlive() const;
	float GetHealth() { return mHealth; }
	void TakeDamage(int dam);
	void ResetHealth();
	void ResetFatigue();
	int GetDamageCap() { return mDamageMax; }
	float GetFatigue() { return mFatigue; }
	void ChangeActiveState();
	void IncreaseFatigue(float val);
	void StartTimer();
	void IterateTimer(float dt);

	
	void DebugUI();

	Stopwatch myTimer;


private:
	AI::StateMachine<Enemy>* mStateMachine;
	LocationState mLocation;
	bool mActive = false;
	bool mEnemyFound = false;
	float mHealth;
	float mHealthCap;
	int mDamageMax;
	float mFatigue;
};




