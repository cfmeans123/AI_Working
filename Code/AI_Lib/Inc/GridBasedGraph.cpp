#include "Precompiled.h"
#include "GridBasedGraph.h"

using namespace AI;

void AI::GridBasedGraph::Initialize(int columns, int rows)
{
	mNodes.clear();
	mNodes.reserve(columns * rows);
	mColumns = columns;
	mRows = rows;
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			auto& node = mNodes.emplace_back();
			node.column = j;
			node.row = i;
		}
	}
}

void GridBasedGraph::ResetSearchParams()
{
	for (auto& node : mNodes)
	{
		node.Reset();
	}
}

GridBasedGraph::Node* GridBasedGraph::GetNode(int x, int y)
{
	if (x >= 0 && x < mColumns && y >= 0 && y < mRows)
	{
		const int index = GetIndex(x, y);
		if (index < mNodes.size())
		{
			return &mNodes[index];
		}
	}
	return nullptr;
}



const GridBasedGraph::Node* GridBasedGraph::GetNode(int x, int y) const
{
	if (x >= 0 && x < mColumns && y >= 0 && y < mRows)
	{
		const int index = GetIndex(x, y);
		if (index < mNodes.size())
		{
			return &mNodes[index];
		}
	}
	return nullptr;
}


int GridBasedGraph::GetIndex(int x, int y) const
{
	return x + (y * mColumns);
}