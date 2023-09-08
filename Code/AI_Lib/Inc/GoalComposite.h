#pragma once

#include "Goal.h"

namespace AI
{
	template<class AgentType>
	class GoalComposite : public Goal<AgentType>
	{
	public:
		using Status = Goal<AgentType>::Status;


		GoalComposite(AgentType& agent)
			: Goal<AgentType>(agent)
		{
		
		}
		virtual ~GoalComposite()
		{
			RemoveAllSubgoals();
		}

	protected:
		template<class GoalType>
		void AddSubGoal()
		{
			static_assert(std::is_base_of_v<Goal, GoalType>, "GoalComposite: must be derived from type 'Goal' ");
			mSubGoals.push_front(std::make_unique<GoalType>());
		}

		void RemoveAllSubgoals()
		{
			for (auto& goal : mSubGoals)
			{
				goal->Terminate();
			}
			mSubGoals.clear();
		}

		Status ProcessSubgoals()
		{
			while (!mSubGoals.empty())
			{
				auto& goal = mSubGoals.front();
				if (goal->mStatus != Status::Completed
					&& goal->mStatus != Status::Failed)
				{
					break;
				}
			}

			if (!mSubGoals.empty())
			{
				Status status = mSubGoals.front()->Process();
				if (status == Status::Completed && mSubGoals.size() > 1)
				{
					return Status::Active;
				}
				return status;
			}
			return Status::Completed;
		}

		using SubGoals = std::list<std::unique_ptr<Goal<AgentType>>>;
		SubGoals mSubGoals;
	};
}