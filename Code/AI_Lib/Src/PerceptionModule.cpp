#include "Precompiled.h"
#include "PerceptionModule.h"

using namespace AI;

PerceptionModule::PerceptionModule(Agent& agent, ImportanceCalculator calculator)
	: mAgent(agent)
	, ComputeImportance(calculator)
{

}

void PerceptionModule::update(float deltaTime)
{
	//update all the sensors
	for (auto& sensor : mSensors)
	{
		sensor->Update(mAgent, mMemoryRecords, deltaTime);
	}

	//remove old memory records
	mMemoryRecords.erase(
		std::remove_if(
			mMemoryRecords.begin(),
			mMemoryRecords.end(),
			[span = mMemorySpan](const auto& record)
			{
				return record.lastRecordedTime + span < X::GetTime();
			}), mMemoryRecords.end());
	//calculate importance of remaining records
	for (auto& record : mMemoryRecords)
	{
		record.importance = ComputeImportance(mAgent, record);
	}

	//sory by importance
	mMemoryRecords.sort(
		[](const auto& a, const auto& b)
		{
			return a.importance > b.importance;
		});


}