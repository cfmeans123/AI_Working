#pragma once
#pragma once

#include "../AI_Lib/Inc/AI.h"

#include "Peon.h"
#include "Mineral.h"
#include "ImGui/Inc/imgui.h"

bool atMineral = false;
int debugRetCount;
AI::MemoryRecord target;
;
X::Math::Vector2 lastpos;


class IdleState : public AI::State<Peon>
{
public:
	bool idleTimerComplete;
	float idleTimer;
	void Enter(Peon& agent) override
	{
		idleTimer = 3.0f;
		agent.homepos = X::RandomVector2({ 100.0f, 100.0f }, { X::GetScreenWidth() - 100.0f, X::GetScreenHeight() - 100.0f });
		agent.destination = X::RandomVector2({ 100.0f, 100.0f }, { X::GetScreenWidth() - 100.0f, X::GetScreenHeight() - 100.0f });
	}
	void Update(Peon& agent, float deltaTime) override
	{
		idleTimer -= deltaTime;
		if (idleTimer < 0.0f)
		{
			agent.ChangeState(PeonState::Patrol);
		}
	}
	void Exit(Peon& agent) override
	{

	}
	void DebugUI() override
	{
		ImGui::Text("Your current state is: IdleState");
	}
};

class PatrolState : public AI::State<Peon>
{
public:
	float compare = 0.0f;

	void Enter(Peon& agent) override
	{
		if (agent.returnCount > 4)
		{
			agent.ChangeState(PeonState::Destroy);
		}
		else
		{
			agent.SetSeek(true);
		}
	}
	void Update(Peon& agent, float deltaTime) override
	{
		const auto& memoryRecords = agent.GetMemoryRecord();
		
		for (auto& memory : memoryRecords)
		{

			compare = memory.importance;
			agent.destination = memory.GetProperty<X::Math::Vector2>("lastSeenPosition");

			if (memory.importance > compare)
			{
				target = memory;
				agent.destination = memory.GetProperty<X::Math::Vector2>("lastSeenPosition");
			}

		}

		if (X::Math::IsZero(agent.destination - target.GetProperty<X::Math::Vector2>("lastSeenPosition")))
		{
			agent.ChangeState(PeonState::Mine);
		}
		if (X::Math::Distance(agent.destination, agent.position) < 200.0f)
		{
			if (memoryRecords.empty())
			{
				agent.ChangeState(PeonState::Recover);
			}
			else
			{
				agent.ChangeState(PeonState::Mine);
			}
		}

	}
	void Exit(Peon& agent) override
	{
		//agent.SetArrive(false);
		agent.SetSeek(false);
	}
	void DebugUI() override
	{
		ImGui::Text("Your current state is: PatrolState");
	}
};

class EngageState : public AI::State<Peon>
{
public:


	void Enter(Peon& agent) override
	{

	}
	void Update(Peon& agent, float deltaTime) override
	{

	}
	void Exit(Peon& agent) override
	{

	}
	void DebugUI() override
	{
		ImGui::Text("Your current state is: EngageState");
	}
};

class RecoverState : public AI::State<Peon>
{
public:
	const float screenWidth = X::GetScreenWidth();
	const float screenHeight = X::GetScreenHeight();
	void Enter(Peon& agent) override
	{

		agent.destination = X::RandomVector2({ 100.0f, 100.0f }, { screenWidth - 100.0f, screenHeight - 100.0f });

		agent.SetSeek(true);

	}
	void Update(Peon& agent, float deltaTime) override
	{
		const auto& memoryRecords = agent.GetMemoryRecord();

		for (auto& memory : memoryRecords)
		{
			if (memory.importance != 0)
			{
				agent.ChangeState(PeonState::Patrol);
			}
		}
		if (X::Math::Distance(agent.destination, agent.position) < 10.0f)
		{
			agent.ChangeState(PeonState::Patrol);
		}
	}
	void Exit(Peon& agent) override
	{
		agent.SetSeek(false);
	}
	void DebugUI() override
	{
		ImGui::Text("Your current state is: RecoverState");
	}
};
class DestroyState : public AI::State<Peon>
{
public:
	void Enter(Peon& agent) override
	{
		agent.SetFlee(true);
		agent.target = dynamic_cast<AI::Agent*>(agent.world.GetEntities().at(0));
	}
	void Update(Peon& agent, float deltaTime) override
	{

	}
	void Exit(Peon& agent) override
	{

	}
	void DebugUI() override
	{
		ImGui::Text("Your current state is: DestroyState. How unlucky can you be?");
	}
};
class MineState : public AI::State<Peon>
{
public:
	float mineTimer = 3.0f;
	void Enter(Peon& agent) override
	{
		agent.SetArrive(true);
		lastpos = agent.destination;
	}
	void Update(Peon& agent, float deltaTime) override
	{
		if (Distance(agent.destination, agent.position) < 1.0f)
		{
			mineTimer -= deltaTime;
			if (mineTimer <= 0.0f)
			{
				agent.ChangeState(PeonState::Return);
			}
		}
	}
	void Exit(Peon& agent) override
	{
		agent.SetArrive(false);
	}
	void DebugUI() override
	{
		ImGui::Text("Your current state is: MineState.");
	}
};
class ReturnState : public AI::State<Peon>
{
public:
	void Enter(Peon& agent) override
	{

		agent.destination = agent.homepos;
		debugRetCount = agent.returnCount;
		agent.SetArrive(true);
	}
	void Update(Peon& agent, float deltaTime) override
	{
		if (Distance(agent.destination, agent.position) < 1.0f)
		{
			agent.destination = lastpos;
			agent.ChangeState(PeonState::Patrol);
		}
	}
	void Exit(Peon& agent) override
	{
		agent.returnCount++;
	}
	void DebugUI() override
	{
		ImGui::Text("Your current state is: ReturnState.\n");
		ImGui::Text("Your current return cont is: [%d]", debugRetCount);
	}
};