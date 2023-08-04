#pragma once
#include <chrono> 

class Stopwatch
{
public:
	float mTime;
	bool mActive = false;

	void ChangeActiveState();
	bool GetActiveState();
	void ResetTimer(float time);
	void Update(float dt);
};