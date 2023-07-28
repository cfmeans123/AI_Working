#include "Precompiled.h"
#include "AStar.h"

using namespace AI;

bool AStar::Run(GridBasedGraph& graph, int startX, int startY, int endX, int endY, GetCost getCost, GetHeuristic heuristic)
{
	graph.ResetSearchParams();
	mOpenList.clear();
	mClosedList.clear();

	//Add start node to open list
	auto node = graph.GetNode(startX, startY);
	auto endNode = graph.GetNode(endX, endY);
	mOpenList.push_back(node);
	node->opened = true;

	

	bool found = false;
	//search for the end node
	while (!found && !mOpenList.empty())
	{
		//BFS is First In First Out, use Queue data structure
		node = mOpenList.front();
		mOpenList.pop_front();

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
				if (neighbor == nullptr || neighbor->closed)
				{
					continue;
				}
				float cost = node->cost + getCost(node, neighbor);
				if (!neighbor->opened)
				{
					//do a sorted insert
					for (NodeList::iterator it = mOpenList.begin(); it != mOpenList.end(); ++it)
					{
						if ((*it)->cost > cost)
						{
							mOpenList.insert(it, neighbor);
						}
					}
					mOpenList.push_back(neighbor);
					neighbor->opened = true;
					neighbor->parent = node;
					neighbor->cost = node->cost + getCost(node, neighbor);
					neighbor->heuristic = heuristic(neighbor, endNode);
				}
				else if (cost < neighbor->cost)
				{
					//edge relaxation, changes the neighbor parent
					neighbor->parent = node;
					neighbor->cost = cost;
				}
			}
		}
		//Lambdas can be declared within parameter lists
		auto sortCost = [](const GridBasedGraph::Node* a, const GridBasedGraph::Node* b)
		{
			if (a->cost == b->cost)
			{
				return a->heuristic < b->heuristic;
			}
			return a->cost < b->cost;
		};
		mOpenList.sort(sortCost);
		//std::sort(mOpenList.begin(), mOpenList.end(), sortCost);
		//add the processed node to the closed list
		mClosedList.push_back(node);
		node->closed;
	}

	return found;
}