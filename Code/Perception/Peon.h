#pragma once

#include "../AI_Lib/Inc/AI.h"

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


private:
	std::unique_ptr<AI::SteeringModule> mSteeringModule;
	AI::WanderBehavior* mWanderBehavior = nullptr;
	AI::ArriveBehavior* mArriveBehavior = nullptr;
	AI::EvadeBehavior* mEvadeBehavior = nullptr;
	AI::PursuitBehavior* mPursuitBehavior = nullptr;
	AI::FleeBehavior* mFleeBehavior = nullptr;
	AI::SeekBehavior* mSeekBehavior = nullptr;
	AI::SeparationBehavior* mSeparationBehavior = nullptr;
	AI::AlignmentBehavior* mAlignmentBehavior = nullptr;
	AI::CohesionBehavior* mCohesionBehavior = nullptr;


	std::array<X::TextureId, 16> mTextureIds;

};