#pragma once

#include "Grid.h"

class Pathfinding
{
public:
	Pathfinding() = default;
	Pathfinding(Grid& grid);

	void FindPath(const sf::Vector2f& startPos, const sf::Vector2f& targetPos);
	void RetracePath(const Node& startNode, Node& targetNode);
	int GetDistance(const Node& nodeA, const Node& nodeB);
private:
	Grid& m_Grid;
};
