#pragma once

namespace AI
{
	template <class AgentType>
	class State
	{
	public:
		virtual ~State() = default;
		virtual void Enter(AgentType& agent) = 0;
		virtual void Execute(AgentType& agent, float deltaTime) = 0;
		virtual void Exit(AgentType& agent) = 0;
	};

}