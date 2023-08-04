#include "Stopwatch.h"



void Stopwatch::ChangeActiveState()
{
	mActive = !mActive;
}

bool Stopwatch::GetActiveState()
{
	return mActive;
}

void Stopwatch::Update(float dt)
{
	if (GetActiveState())
	{
		if (mTime >= 0)
		{
			mTime -= dt;
		}
		else
		{
			ResetTimer(10.0f);
		}
	}
}
void Stopwatch::ResetTimer(float time)
{
	mTime = time;
	ChangeActiveState();
}