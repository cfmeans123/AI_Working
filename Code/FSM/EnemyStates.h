#pragma once

#include "AI.h"

#include "Enemy.h"




class IdleState : public AI::State<Enemy> 
{
public:
	void Enter(Enemy& agent) override
	{
		agent.SetLocation(Enemy::LocationState::Alleys);
	}
	void Execute(Enemy& agent, float deltaTime) override
	{
		if (agent.mActive)
		{
			agent.ChangeState(EnemyStates::Patrol);
		}
	}
	void Exit(Enemy& agent) override
	{
		
	}
};

class PatrolState : public AI::State<Enemy>
{
public:
	void Enter(Enemy& agent) override
	{

	}
	void Execute(Enemy& agent, float deltaTime) override
	{

	}
	void Exit(Enemy& agent) override
	{

	}
};

class EngageState : public AI::State<Enemy>
{
public:
	void Enter(Enemy& agent) override
	{

	}
	void Execute(Enemy& agent, float deltaTime) override
	{

	}
	void Exit(Enemy& agent) override
	{

	}
};

class RecoverState : public AI::State<Enemy>
{
public:
	void Enter(Enemy& agent) override
	{

	}
	void Execute(Enemy& agent, float deltaTime) override
	{

	}
	void Exit(Enemy& agent) override
	{

	}
};
class DestroyState : public AI::State<Enemy>
{
public:
	void Enter(Enemy& agent) override
	{

	}
	void Execute(Enemy& agent, float deltaTime) override
	{

	}
	void Exit(Enemy& agent) override
	{

	}
};