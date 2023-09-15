#pragma once
#pragma once

#include "../AI_Lib/Inc/AI.h"

#include "Peon.h"
#include "Mineral.h"
#include "ImGui/Inc/imgui.h"


class IdlingState : public AI::State<Hunter>
{
public:

	void Enter(Hunter& agent) override
	{

	}
	void Update(Hunter& agent, float deltaTime) override
	{

	}
	void Exit(Hunter& agent) override
	{

	}
	void DebugUI() override
	{
		ImGui::Text("Hunter current state is: IdlingState");
	}
};

class HuntingState : public AI::State<Hunter>
{
public:

	void Enter(Hunter& agent) override
	{

	}
	void Update(Hunter& agent, float deltaTime) override
	{

	}
	void Exit(Hunter& agent) override
	{

	}
	void DebugUI() override
	{
		ImGui::Text("Hunter current state is: HuntingState");
	}
};

class ChasingState : public AI::State<Hunter>
{
public:


	void Enter(Hunter& agent) override
	{

	}
	void Update(Hunter& agent, float deltaTime) override
	{

	}
	void Exit(Hunter& agent) override
	{

	}
	void DebugUI() override
	{
		ImGui::Text("Hunter current state is: ChasingState");
	}
};

class EngagingState : public AI::State<Hunter>
{
public:
	void Enter(Hunter& agent) override
	{
	}
	void Update(Hunter& agent, float deltaTime) override
	{

	}
	void Exit(Hunter& agent) override
	{

	}
	void DebugUI() override
	{
		ImGui::Text("Hunter current state is: EngagingState");
	}
};
