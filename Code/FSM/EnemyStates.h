#pragma once

#include "AI.h"

#include "Enemy.h"

#include "ImGui/Inc/imgui.h"




class IdleState : public AI::State<Enemy> 
{
public:
	bool idleTimerComplete;

	void Enter(Enemy& agent) override
	{
		idleTimerComplete = false;
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
					agent.IncreaseFatigue(0.5f);
					if (f == 5 || f == 6)
					{
						agent.ChangeState(Engage);
						agent.myTimer.ResetTimer(0.0f);
					}
				}
			}
			//change to check route complete in the future
			if (patrolTimerComplete)
			{
				//agent.ChangeState(EnemyState::Patrol);
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
	//in place of attack speed
	float f_max;
	float f;
	int damCap;	
	int debugHealth;
	int debugDamage;
	int otherDebugDamage;

	//get enemy stats...init stats with function for cleaner implementation
	int otherDamageMax;
	int otherHealth;
	bool otherAlive;

	void Enter(Enemy& agent) override
	{
		//get my stats - health, atk speed, damage, stamina, etc...
		f_max = 1;
		f = 0;
		damCap = agent.GetDamageCap();
		debugHealth = agent.GetHealth();
		
		//get current enemy stats...
		otherDamageMax = rand() % 20 + 5;
		/*while (otherDamageMax == 0){
			otherDamageMax = rand() & 25;
		}*/
		otherHealth = 100;
		otherAlive = true;
	}
	void Update(Enemy& agent, float deltaTime) override
	{
		if (agent.IsAlive() && otherAlive)
		{
			f += deltaTime;

			if (f > f_max)
			{
				//instead, iterate f_max with attack speed variable and create separate loop for enemy damage
				f_max += 1;
				//check attack speed to determine who swings first...
				debugDamage = rand() % damCap;
				otherHealth -= debugDamage;
				agent.IncreaseFatigue(1.5f);
				if (otherHealth <= 0)
				{
					otherAlive = false;					
				}

				if (otherAlive)
				{
					otherDebugDamage = rand() % otherDamageMax;
					agent.TakeDamage(otherDebugDamage);
				}
				debugHealth = agent.GetHealth();
			}
		}
		else if (!agent.IsAlive() && otherAlive)
		{
			agent.ChangeState(EnemyState::Destroy);
		}
		else if (agent.IsAlive() && !otherAlive)
		{
			agent.ChangeState(EnemyState::Recover);
		}
	}
	void Exit(Enemy& agent) override
	{
		
	}
	void DebugUI() override
	{
		ImGui::Text("Your current state is: EngageState");
		ImGui::Text("Your health is: [%d]\nEnemy Health is: [%d]\nYou attack the enemy for: [%d]\nEnemy attacks you for: [%d]", debugHealth, otherHealth, debugDamage, otherDebugDamage);
	}
};

class RecoverState : public AI::State<Enemy>
{
public:
	float curFatigue;

	void Enter(Enemy& agent) override
	{
		curFatigue = agent.GetFatigue();
	}
	void Update(Enemy& agent, float deltaTime) override
	{
		if (curFatigue > 0.0f)
		{
			curFatigue -= deltaTime;
		}
		else
		{
			agent.ChangeState(EnemyState::Patrol);

		}
	}
	void Exit(Enemy& agent) override
	{
		agent.ResetHealth();
		agent.ResetFatigue();
	}
	void DebugUI() override
	{
		ImGui::Text("Your current state is: RecoverState");
		ImGui::Text("Resting time remaining: [%f]", curFatigue);
	}
};
class DestroyState : public AI::State<Enemy>
{
public:
	bool destroyTimerComplete;
	void Enter(Enemy& agent) override
	{
		destroyTimerComplete = false;
	}
	void Update(Enemy& agent, float deltaTime) override
	{
		if (agent.IsActive())
		{
			if (!agent.myTimer.GetActiveState())
			{
				agent.myTimer.ResetTimer(5.0f);
			}
			else
			{
				agent.myTimer.mTime -= deltaTime;
				if (agent.myTimer.mTime <= 0.0f)
				{
					destroyTimerComplete = true;
				}
			}
			if (destroyTimerComplete)
			{
				agent.ChangeState(EnemyState::Idle);
			}
		}
	}
	void Exit(Enemy& agent) override
	{
		agent.ResetHealth();
		agent.ResetFatigue();
	}
	void DebugUI() override
	{
		ImGui::Text("Your current state is: DestroyState. How unlucky can you be?");
	}
};