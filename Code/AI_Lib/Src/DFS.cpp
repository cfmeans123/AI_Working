#include "Precompiled.h"
#include "DFS.h"

using namespace AI;

bool DFS::Run(GridBasedGraph& graph, int startX, int startY, int endX, int endY)
{
	graph.ResetSearchParams();
	mOpenList.clear();
	mClosedList.clear();

	//Add start node to open list
	auto node = graph.GetNode(startX, startY);
	mOpenList.push_back(node);
	node->opened = true;

	bool found = false;
	//search for the end node
	while (!found && !mOpenList.empty())
	{
		//BFS is First In First Out, use Queue data structure
		node = mOpenList.back();
		mOpenList.pop_back();

		//check if this is the end node?
		if (node->column == endX && node->row == endY)
		{
			found = true;
		}
		else
		{
			//go through the neighbors and add to the open list
			for (auto neighbor : node->neighbors)
			{
				if (neighbor == nullptr || neighbor->opened)
				{
					continue;
				}
				mOpenList.push_back(neighbor);
				neighbor->opened = true;
				neighbor->parent = node;
			}
		}
		//add the processed node to the closed list
		mClosedList.push_back(node);
		node->closed;
	}


	return found;
}