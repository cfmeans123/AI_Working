#pragma once

#include "AI.h"

#include "Enemy.h"

#include "ImGui/Inc/imgui.h"




class IdleState : public AI::State<Enemy> 
{
public:
	bool idleTimerComplete = false;

	void Enter(Enemy& agent) override
	{
		agent.SetLocation(Enemy::LocationState::Alleys);
	}
	void Update(Enemy& agent, float deltaTime) override
	{
		if (agent.IsActive())
		{
			if (!agent.myTimer.GetActiveState() && !idleTimerComplete)
			{
				agent.myTimer.ResetTimer(5.0f);
			}
			else if (agent.myTimer.GetActiveState() && !idleTimerComplete)
			{
				agent.myTimer.mTime -= deltaTime;
				if (agent.myTimer.mTime <= 0.0f)
				{
					agent.myTimer.ResetTimer(5.0f);
					idleTimerComplete = true;
				}
			}
			if (idleTimerComplete)
			{				
				agent.ChangeState(EnemyState::Patrol);
			}
		}
	}
	void Exit(Enemy& agent) override
	{
		idleTimerComplete = false;
	}
	void DebugUI() override
	{
		ImGui::Text("Your current state is: IdleState");
	}
};

class PatrolState : public AI::State<Enemy>
{
public:
	bool patrolTimerComplete = false;
	const int f_max = 10;
	int f;
	int patrolDuration = 10.0f;

	void Enter(Enemy& agent) override
	{
		agent.SetLocation(Enemy::LocationState::Streets);
	}
	void Update(Enemy& agent, float deltaTime) override
	{
		if (agent.IsActive())
		{
			if (!agent.myTimer.GetActiveState() && !patrolTimerComplete)
			{
				agent.myTimer.ResetTimer(10.0f);
			}
			else if (agent.myTimer.GetActiveState() && !patrolTimerComplete)
			{
				agent.myTimer.mTime -= deltaTime;
				if (agent.myTimer.mTime <= 0.0f)
				{
					agent.myTimer.ResetTimer(patrolDuration);
					patrolTimerComplete = true;
				}
				if (agent.myTimer.mTime > 0.0f && agent.myTimer.mTime < patrolDuration - 1 && patrolDuration > 0.0f)
				{
					f = rand() % f_max;
					--patrolDuration;
					if (f == 5)
					{
						agent.ChangeState(Engage);
						agent.myTimer.ResetTimer(10.0f);
					}
				}
				
			}
			if (patrolTimerComplete)
			{
				agent.ChangeState(EnemyState::Patrol);
				patrolTimerComplete = false;
				patrolDuration = 10.0f;
			}
		}
	}
	void Exit(Enemy& agent) override
	{
		patrolDuration = 10.0f;
	}
	void DebugUI() override
	{
		ImGui::Text("Your current state is: PatrolState");
		ImGui::Text("Random number is: [%d]", f);
	}
};

class EngageState : public AI::State<Enemy>
{
public:
	

	void Enter(Enemy& agent) override
	{

	}
	void Update(Enemy& agent, float deltaTime) override
	{

	}
	void Exit(Enemy& agent) override
	{

	}
	void DebugUI() override
	{
		ImGui::Text("Your current state is: EngageState");
	}
};

class RecoverState : public AI::State<Enemy>
{
public:
	void Enter(Enemy& agent) override
	{

	}
	void Update(Enemy& agent, float deltaTime) override
	{

	}
	void Exit(Enemy& agent) override
	{

	}
	void DebugUI() override
	{
		ImGui::Text("Your current state is: RecoverState");
	}
};
class DestroyState : public AI::State<Enemy>
{
public:
	void Enter(Enemy& agent) override
	{

	}
	void Update(Enemy& agent, float deltaTime) override
	{

	}
	void Exit(Enemy& agent) override
	{

	}
	void DebugUI() override
	{
		ImGui::Text("Your current state is: DestroyState");
	}
};