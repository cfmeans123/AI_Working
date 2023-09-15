#pragma once

#include "../AI_Lib/Inc/AI.h"

enum PeonState
{
	Idle,
	Patrol,
	Engage,
	Recover,
	Destroy,
	Mine,
	Return
};

class VisualSensor;
class Peon : public AI::Agent
{
public:
	Peon(AI::AIWorld& world);

	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render();

	void ShowDebug(bool debug);
	void SetArrive(bool active) { mArriveBehavior->SetActive(active); }
	void SetEvade(bool active) {   mEvadeBehavior->SetActive(active); }
	void SetPursuit(bool active) { mPursuitBehavior->SetActive(active); }
	void SetFlee(bool active) {    mFleeBehavior->SetActive(active); }
	void SetSeek(bool active) {    mSeekBehavior->SetActive(active); }
	void SetWander(bool active) {  mWanderBehavior->SetActive(active); }
	AI::MemoryRecords GetMemoryRecord() { return mPerceptionModule->GetMemoryRecords(); }
	AI::StateMachine<Peon>* GetStateMachine() { return mStateMachine; }
	void Initialize();
	void Terminate();
	void ChangeState(PeonState newState);
	void DebugUI();
	X::Math::Vector2 homepos;
	int returnCount = 0;




private:
	std::unique_ptr<AI::SteeringModule> mSteeringModule;
	std::unique_ptr<AI::PerceptionModule> mPerceptionModule;
	AI::WanderBehavior* mWanderBehavior = nullptr;
	AI::ArriveBehavior* mArriveBehavior = nullptr;
	AI::EvadeBehavior* mEvadeBehavior = nullptr;
	AI::PursuitBehavior* mPursuitBehavior = nullptr;
	AI::FleeBehavior* mFleeBehavior = nullptr;
	AI::SeekBehavior* mSeekBehavior = nullptr;
	AI::SeparationBehavior* mSeparationBehavior = nullptr;
	AI::AlignmentBehavior* mAlignmentBehavior = nullptr;
	AI::CohesionBehavior* mCohesionBehavior = nullptr;
	AI::ObstacleAvoidanceBehavior* mObstacleAvoidanceBehavior = nullptr;

	VisualSensor* mVisualSensor = nullptr;
	std::array<X::TextureId, 16> mTextureIds;

	AI::StateMachine<Peon>* mStateMachine;



};