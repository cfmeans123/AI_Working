#include "TileMap.h"
#include "ImGui/Inc/imgui.h"
#include <XEngine.h> // <> for external includes, "" for internal includes

TileMap tileMap;
Path path;
int startX = 0;
int startY = 0;
int endX = 0;
int endY = 0;
X::TextureId textureId;
X::Math::Vector2 position;



//--------------------------------------------------

void GameInit()
{
	tileMap.LoadTiles("tiles.txt");
	tileMap.LoadMap("map.txt");

	/*textureId = X::LoadTexture("bird1.png");
	position = { 100.0f, 100.0f };*/
}

bool GameLoop(float deltaTime)
{
	const float moveSpeed = 200.0f; // pixel per second
	if (X::IsKeyDown(X::Keys::RIGHT))
		position.x += moveSpeed * deltaTime;
	if (X::IsKeyDown(X::Keys::DOWN))
		position.y += moveSpeed * deltaTime;
	if (X::IsKeyDown(X::Keys::LEFT))
		position.x -= moveSpeed * deltaTime;
	if (X::IsKeyDown(X::Keys::UP))
		position.y -= moveSpeed * deltaTime;

	if (X::IsMousePressed(0))
	{
		int x = X::GetMouseScreenX();
		int y = X::GetMouseScreenY();
		auto node = tileMap.GetClosestNode(x, y);
		if (node != nullptr)
		{
			startX = node->column;
			startY = node->row;
		}
	}
	if (X::IsMousePressed(1))
	{
		int x = X::GetMouseScreenX();
		int y = X::GetMouseScreenY();
		auto node = tileMap.GetClosestNode(x, y);
		if (node != nullptr)
		{
			endX = node->column;
			endY = node->row;
		}
	}

	ImGui::Begin("TEST BOX");
	//the ## turns it into a key so it stores it as its own individual value
	ImGui::DragInt("StartX##", &startX, 1, 0, tileMap.GetColumns() - 1);
	ImGui::DragInt("StartY##", &startY, 1, 0, tileMap.GetRows() - 1);
	ImGui::DragInt("EndX##", &endX, 1, 0, tileMap.GetColumns() - 1);
	ImGui::DragInt("EndY##", &endY, 1, 0, tileMap.GetRows() - 1);
	if (ImGui::Button("RunBFS##"))
	{
		path = tileMap.FindPathBFS(startX, startY, endX, endY);
	}
	if (ImGui::Button("RunDFS##"))
	{
		path = tileMap.FindPathDFS(startX, startY, endX, endY);
	}
	if (ImGui::Button("RunDijkstra##"))
	{
		path = tileMap.FindPathDijkstra(startX, startY, endX, endY);
	}
	if (ImGui::Button("RunAStar##"))
	{
		path = tileMap.FindPathAStar(startX, startY, endX, endY);
	}

	ImGui::LabelText("Test Label 1", "It works!");
	ImGui::End();

	if (X::IsKeyPressed(X::Keys::ONE))
	{
		path = tileMap.FindPathBFS(startX, startY, endX, endY);
	}

	tileMap.Render();
	for (int i = 1; i < path.size(); ++i)
	{
		X::DrawScreenLine(path[i - 1], path[i], X::Colors::Red);
	}
	X::DrawScreenCircle(tileMap.GetPixelPosition(startX, startY), 10, X::Colors::Yellow);
	X::DrawScreenCircle(tileMap.GetPixelPosition(endX, endY), 10, X::Colors::Red);

	/*X::DrawSprite(textureId, position, X::Pivot::TopLeft, X::Flip::Horizontal);
	X::DrawScreenDiamond(position, 5.0f, X::Colors::Cyan);*/

	const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);
	return quit;
}

void GameCleanup()
{

}

//--------------------------------------------------

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("Pathfinding");
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
	return 0;
}