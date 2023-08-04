#include <XEngine.h> // <> for external includes, "" for internal includes
#include "ImGui/Inc/imgui.h"
#include "Enemy.h"

//--------------------------------------------------
Enemy enemy;

void GameInit()
{
	enemy.Initialize();
}

bool GameLoop(float deltaTime)
{

 	enemy.Update(deltaTime);
	if (X::IsKeyPressed(X::Keys::SPACE))
	{
	}
	ImGui::Begin("FSM");
	enemy.DebugUI();
	ImGui::End();
	const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);
	return quit;
}

void GameCleanup()
{

}

//--------------------------------------------------

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	srand(time(0));

	X::Start("FSM");
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
	return 0;
}