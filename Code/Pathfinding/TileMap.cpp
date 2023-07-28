#include "TileMap.h"

using namespace AI;

namespace
{
	inline int ToIndex(int x, int y, int columns)
	{
		return x + (y * columns);
	}
}

void TileMap::LoadTiles(const char* fileName)
{
	// TODO - Read the provided file and populate mTiles here
	//read out all the tile data
	// string for the texture
	// std::string textyreStr;
	//store in mTiles
	if (!std::filesystem::exists(fileName))
	{
		return;
	}

	std::fstream file(fileName);

	int count = 0;
	std::string buffer;

	file >> buffer;
	file >> count;


	mTiles.clear();
	mTiles.reserve(count);


	for (int i = 0; i < count; ++i)
	{
		int isBlocked = 0;
		file >> buffer;
		file >> isBlocked;

		auto& tile = mTiles.emplace_back();
		tile.isBlocked = isBlocked == 1;
		tile.textureId = X::LoadTexture(buffer.c_str());
	}
	file.close();
	mTileWidth = X::GetSpriteWidth(mTiles[0].textureId);
	mTileHeight = X::GetSpriteHeight(mTiles[0].textureId);
}

void TileMap::LoadMap(const char* fileName)
{
	if (!std::filesystem::exists(fileName))
	{
		return;
	}
	std::fstream file(fileName);
	int columns = 0, rows = 0;
	std::string buffer;

	file >> buffer;
	file >> columns;
	file >> buffer;
	file >> rows;

	mRows = rows;
	mColumns = columns;

	mMap.resize(columns * rows);
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			char tileType;
			file >> tileType;
			mMap[ToIndex(j, i, columns)] = tileType - '0';
		}
	}
	file.close();

	mGraph.Initialize(mColumns, mRows);
	auto GetNeighbor = [&](int c, int r) -> GridBasedGraph::Node*
	{
		if (IsBlocked(c, r))
		{
			return nullptr;
		}
		return mGraph.GetNode(c, r);
	};
	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mColumns; j++)
		{
			if (IsBlocked(j, i))
			{
				continue;
			}

			//Filter out the nodes that are blocked to save processor time, using the lambda above
			GridBasedGraph::Node* node = mGraph.GetNode(j, i);
			node->neighbors[GridBasedGraph::Direction::North] = GetNeighbor(j, i - 1);
			node->neighbors[GridBasedGraph::Direction::South] = GetNeighbor(j, i + 1);
			node->neighbors[GridBasedGraph::Direction::East] = GetNeighbor(j + 1, i);
			node->neighbors[GridBasedGraph::Direction::West] = GetNeighbor(j - 1, i);
			node->neighbors[GridBasedGraph::Direction::NorthEast] = GetNeighbor(j + 1, i - 1);
			node->neighbors[GridBasedGraph::Direction::NorthWest] = GetNeighbor(j - 1, i - 1);
			node->neighbors[GridBasedGraph::Direction::SouthEast] = GetNeighbor(j + 1, i + 1);
			node->neighbors[GridBasedGraph::Direction::SouthWest] = GetNeighbor(j - 1, i + 1);
		}
	}
}

void TileMap::Render() const
{
	X::Math::Vector2 position;
	float spriteHeight = X::GetSpriteHeight(mTiles[mMap[0]].textureId);
	float spriteWidth = X::GetSpriteWidth(mTiles[mMap[0]].textureId);
	for (int i = 0; i < mRows; ++i)
	{
		for (int j = 0; j < mColumns; ++j)
		{
			int index = ToIndex(j, i, mColumns);
			X::DrawSprite(mTiles[mMap[index]].textureId, position);
			position.x += mTileWidth;
		}
		position.x = 0.0f;
		position.y += mTileHeight;
	}
	for (int r = 0; r < mRows; ++r)
	{
		for (int c = 0; c < mColumns; ++c)
		{
			const auto node = mGraph.GetNode(c, r);
			for (const auto neighbor : node->neighbors)
			{
				if (neighbor == nullptr)
				{
					continue;
				}
				const auto a = GetPixelPosition(node->column, node->row);
				const auto b = GetPixelPosition(neighbor->column, neighbor->row);
				X::DrawScreenLine(a, b, X::Colors::Blue);
			}
		}
	}
	for (int r = 0; r < mRows; ++r)
	{
		for (int c = 0; c < mColumns; ++c)
		{
			const auto node = mGraph.GetNode(c, r);
			if (node->parent != nullptr)
			{
				const auto a = GetPixelPosition(node->column, node->row);
				const auto b = GetPixelPosition(node->parent->column, node->parent->row);
				X::DrawScreenLine(a, b, X::Colors::Orange);
			}
		}
	}
}

Path TileMap::FindPathBFS(int startX, int startY, int endX, int endY)
{
	Path path;
	BFS bfs;
	if (bfs.Run(mGraph, startX, startY, endX, endY))
	{
		const auto& closedList = bfs.GetClosetList();
		auto node = closedList.back();
		while (node != nullptr)
		{
			path.push_back(GetPixelPosition(node->column, node->row));
			node = node->parent;
		}
		std::reverse(path.begin(), path.end());
	}
	return path;
}

Path TileMap::FindPathDFS(int startX, int startY, int endX, int endY)
{
	Path path;
	DFS dfs;
	if (dfs.Run(mGraph, startX, startY, endX, endY))
	{
		const auto& closedList = dfs.GetClosetList();
		auto node = closedList.back();
		while (node != nullptr)
		{
			path.push_back(GetPixelPosition(node->column, node->row));
			node = node->parent;
		}
		std::reverse(path.begin(), path.end());
	}
	return path;
}

Path TileMap::FindPathDijkstra(int startX, int startY, int endX, int endY)
{
	Path path;
	Dijkstra dijkstra;
	auto getCost = [&](const GridBasedGraph::Node* node, const GridBasedGraph::Node* neighbor)
	{
		//if neighbor 
		if (node->column != neighbor->column && node->row != neighbor->row)
		{
			return 1.71f;
		}
		/*int index = ToIndex(neighbor->column, neighbor->row);
		if (mTiles[mMap[index]])*/
		return 1.0f;
	};
	if (dijkstra.Run(mGraph, startX, startY, endX, endY, getCost))
	{
		const auto& closedList = dijkstra.GetClosedList();
		auto node = closedList.back();
		while (node != nullptr)
		{
			path.push_back(GetPixelPosition(node->column, node->row));
			node = node->parent;
		}
		std::reverse(path.begin(), path.end());
	}
	return path;
}

Path TileMap::FindPathAStar(int startX, int startY, int endX, int endY)
{
	Path path;
	AStar aStar;

	auto getCost = [&](const GridBasedGraph::Node* node, const GridBasedGraph::Node* neighbor)
	{
		//if neighbor 
		if (node->column != neighbor->column && node->row != neighbor->row)
		{
			return 1.71f;
		}
		/*int index = ToIndex(neighbor->column, neighbor->row);
		if (mTiles[mMap[index]])*/
		return 1.0f;
	};

	auto getHeuristic = [](const GridBasedGraph::Node* neighbor, const GridBasedGraph::Node* endNode)->float
	{
		//Manhattan distance
		float D = 1.0f;
		return (D * abs(neighbor->column - endNode->column) + abs(neighbor->row - endNode->row));
	};

	if (aStar.Run(mGraph, startX, startY, endX, endY, getCost, getHeuristic))
	{
		const auto& closedList = aStar.GetClosedList();
		auto node = closedList.back();
		while (node != nullptr)
		{
			path.push_back(GetPixelPosition(node->column, node->row));
			node = node->parent;
		}
		std::reverse(path.begin(), path.end());
	}
	return path;

}

bool TileMap::IsBlocked(int x, int y) const
{
	if (x >= 0 && x < mColumns && y >= 0 && y < mRows)
	{
		int index = ToIndex(x, y, mColumns);
		if (index < mMap.size())
		{
			int tile = mMap[index];
			return mTiles[tile].isBlocked;
		}
	}
	return true;
}


const X::Math::Vector2 TileMap::GetPixelPosition(int x, int y) const
{
	//cleaner way to return the two values in the Vector2
	return
	{
		(x + 0.5f) * mTileWidth,
		(y + 0.5f) * mTileHeight
	};
}

const AI::GridBasedGraph::Node* TileMap::GetClosestNode(int x, int y) const
{
	X::Math::Vector2 mousePos(x, y);
	const AI::GridBasedGraph::Node* clickedNode = nullptr;
	float closestDist = FLT_MAX;
	for (int r = 0; r < mRows; ++r)
	{
		for (int c = 0; c < mColumns; ++c)
		{
			const auto node = mGraph.GetNode(c, r);
			float distanceSqr = X::Math::DistanceSqr(GetPixelPosition(node->column, node->row), mousePos);
			if (distanceSqr < closestDist)
			{
				closestDist = distanceSqr;
				clickedNode = node;
			}
		}
	}
	return clickedNode;
}

// 2D map - 5 columns x 4 rows
// [0][0][0][0][0]
// [0][0][0][0][0]
// [0][0][0][X][0]   X is at (3, 2)
// [0][0][0][0][0]

// Stored as 1D - 5x4 = 20 slots
// [0][0][0][0][0] [0][0][0][0][0] [0][0][0][X][0] [0][0][0][0][0]
//
// index = column + (row * columnCount)
//       = 3 + (2 * 5)
//       = 13