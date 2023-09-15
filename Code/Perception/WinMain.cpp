#include <XEngine.h>
#include <ImGui/Inc/imgui.h>
#include "Mineral.h"
#include "../Perception/Peon.h"
#include "../Perception/TypeIds.h"
#include "../Perception/Hunter.h"
//--------------------------------------------------
AI::AIWorld aiWorld;
std::vector<std::unique_ptr<Peon>> peons;
std::vector<std::unique_ptr<Mineral>> minerals;
std::array<X::TextureId, 5> homeTextureIds;
Hunter targetPeon(aiWorld);

bool showDebug = false;
float wanderJitter = 5.0f;
float wanderRadius = 20.0f;
float wanderDistance = 50.0f;

float viewRange = 300.0f;
float viewAngle = 45.0f;

int activeBehavior = 0;

AI::ArriveBehavior::Deceleration deceleration = AI::ArriveBehavior::Deceleration::Normal;



void SpawnPeon()
{
	auto& peon = peons.emplace_back(std::make_unique<Peon>(aiWorld));

	peon->Load();
	peon->Initialize();
	peon->ShowDebug(showDebug);

	const float screenWidth = X::GetScreenWidth();
	const float screenHeight = X::GetScreenHeight();
	peon->position = X::RandomVector2({ 100.0f, 100.0f }, { screenWidth - 100.0f, screenHeight - 100.0f });
}


void KillPeon()
{
	auto& peon = peons.back();
	peon->Unload();
	peons.pop_back();
}

void GameInit()
{
	aiWorld.Initialize();
	SpawnPeon();
	targetPeon.Load();
	targetPeon.Initialize();
	targetPeon.SetFlee(false);
	targetPeon.SetSeek(false);
	//targetPeon.ChangeState(HunterState::Destroy);
	targetPeon.position = X::RandomVector2({ 100.0f, 100.0f }, { X::GetScreenWidth() - 100.0f, X::GetScreenHeight() - 100.0f });

	for (size_t i = 0; i < homeTextureIds.size(); ++i)
	{
		char name[128];
		sprintf_s(name, "refinery_%02i.png", i + 1);
		homeTextureIds[i] = X::LoadTexture(name);
	}

	for (int i = 0; i < 10; ++i)
	{
		auto& mineral = minerals.emplace_back(std::make_unique<Mineral>(aiWorld));
		mineral->Initialize();
	}
	aiWorld.AddObstacle({230.0f, 300.0f, 50.0f});

	X::Math::Vector2 topLeft(500.0f, 100.0f);
	X::Math::Vector2 topRight(600.0f, 100.0f);
	X::Math::Vector2 bottomLeft(500.0f, 600.0f);
	X::Math::Vector2 bottomRight(600.0f, 600.0f);
	aiWorld.AddWall({ topLeft, topRight });
	aiWorld.AddWall({ topRight, bottomRight });
	aiWorld.AddWall({ bottomLeft, bottomRight });
	aiWorld.AddWall({ bottomLeft, topLeft });

}

bool GameLoop(float deltaTime)
{
	ImGui::Begin("Steering Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::Button("Spawn"))
	{
		SpawnPeon();
	}
	if (ImGui::Button("Kill"))
	{
		KillPeon();
	}
	if (ImGui::Checkbox("ShowDebug", &showDebug))
	{
		for (auto& peon : peons)
		{
			peon->ShowDebug(showDebug);
		}
	}
	static const char* behaviors[] = {
		"Wander",
		"Seek"
	};

	if (ImGui::Combo("ActiveBehavior##", &activeBehavior, behaviors, std::size(behaviors)))
	{
		for (auto& peon : peons)
		{
			peon->SetWander(activeBehavior == 0);
			peon->SetSeek(activeBehavior == 1);
		}
	}


	if (ImGui::CollapsingHeader("Wander##Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Jitter##", &wanderJitter, 0.1f, 0.1f, 10.0f);
		ImGui::DragFloat("Radius##", &wanderRadius, 0.1f, 0.1f, 100.0f);
		ImGui::DragFloat("Distance##", &wanderDistance, 0.1f, 0.1f, 500.0f);
	}

	if (ImGui::CollapsingHeader("Visual##Sensor", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("View##Range", &viewRange, 1.0f, 100.0f, 1000.0f);
		ImGui::DragFloat("View##Angle", &viewAngle, 1.0f, 10.0f, 100.0f);
	}

	ImGui::End();

	if (X::IsMousePressed(X::Mouse::LBUTTON))
	{
		const auto mouseX = static_cast<float>(X::GetMouseScreenX());
		const auto mouseY = static_cast<float>(X::GetMouseScreenY());
		const auto destination = X::Math::Vector2(mouseX, mouseY);

		for (auto& peon : peons)
		{
			peon->destination = destination;
		}
	}

	aiWorld.Update();
	if (peons.back()->returnCount > 0)
	{
		int frame = peons.back()->returnCount;
		if (frame > 5)
		{
			frame = 5;
		}
		X::DrawSprite(homeTextureIds[frame], peons.back()->homepos);
	}
	for (int i = 0; i < minerals.size(); ++i)
	{
		for (auto& peon : peons)
		{
			if (X::Math::Distance(peon->position, minerals[i]->position) < 2.0f)
			{
				minerals.erase(minerals.begin() + i);
			}
		}
	}
	
	for (auto& peon : peons)
	{
		peon->Update(deltaTime);
		peon->DebugUI();
	}
	targetPeon.DebugUI();
	targetPeon.Update(deltaTime);
	for (auto& peon : peons)
	{
		peon->Render();
	}
	targetPeon.Render();
	for (auto& mineral : minerals)
	{
		mineral->Render();
	}

	auto& obstacles = aiWorld.GetObstacles();
	for (auto& obstacle : obstacles)
	{
		X::DrawScreenCircle(obstacle.center, obstacle.radius, X::Colors::Gray);
	}

	auto& walls = aiWorld.GetWalls();
	for (auto& wall : walls)
	{
		X::DrawScreenLine(wall.from, wall.to, X::Colors::Gray);
	}

	const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);
	return quit;
}

void GameCleanup()
{
	for (auto& peon : peons)
	{
		peon->Unload();
	}
}
//--------------------------------------------------


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("Perception");
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
	return 0;
}