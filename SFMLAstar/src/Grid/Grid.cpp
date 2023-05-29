#include "Grid.h"

// Constructor
Grid::Grid(const sf::Vector2f& startingPoint, const sf::Vector2f& gridWorldSize, int diameter)
	: m_StartingPoint(startingPoint), m_GridWorldSize(gridWorldSize), m_NodeDiameter(diameter)
{
	// Calculate diagonal distance using pythagoras
	m_Diagonal = sqrt((m_NodeDiameter * m_NodeDiameter) * 2);

	// Call grid creating function
	CreateGrid();
}

// Create the grid layout
void Grid::CreateGrid()
{
	// Grid layout: 0 stands for a walkable node and 1 for not a walkable node (obstacle)
	int layout[8 * 8] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 1, 0, 0, 0,
		0, 0, 0, 0, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	};

	// Assign starting point in the window & calculate the basis of layout array
	int basis = std::sqrt(sizeof(layout) / sizeof(int));

	// Loop through layout array both x and y axis
	for (int y = 0; y < basis; y++)
	{
		for (int x = 0; x < basis; x++)
		{
			// Calculate nodes actual top-left corner point
			sf::Vector2f actualPos = { m_StartingPoint.x + (m_NodeDiameter * x), m_StartingPoint.y + (m_NodeDiameter * y) };

			// Check if value is either 0 or 1
			if (layout[(y * basis) + x] == 1) // Obstacle
				m_Nodes[{actualPos.x, actualPos.y}] = Node(false, actualPos, m_NodeDiameter);
			else
				m_Nodes[{actualPos.x, actualPos.y}] = Node(true, actualPos, m_NodeDiameter);
		}
	}
}

// Given world (window) point, return correct node based on its top-left corner coordinates
Node& Grid::NodeFromWorldPoint(const sf::Vector2f& worldPosition)
{
	int differenceX = (worldPosition.x - m_StartingPoint.x);
	int differenceY = (worldPosition.y - m_StartingPoint.y);
	int diffDividedX = differenceX / m_NodeDiameter;
	int diffDividedY = differenceY / m_NodeDiameter;

	if (differenceX >= m_NodeDiameter)
		differenceX -= m_NodeDiameter * diffDividedX;

	if (differenceY >= m_NodeDiameter)
		differenceY -= m_NodeDiameter * diffDividedY;

	int x = worldPosition.x - differenceX;
	int y = worldPosition.y - differenceY;

	return m_Nodes[{(float)x, (float)y}];
}

// Scan 3x3 area of given node to get its neighbours
std::vector<Node*> Grid::GetNeighbours(const Node& node)
{
	std::vector<Node*> neighbours;

	// Iterate through both y and x axis from -1 to 1 (left to right)
	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			// Node itself, pass
			if (x == 0 && y == 0)
				continue;

			// Calculate node's top-left corner position
			auto& worldPos = node.GetWorldPosition();

			int checkX = worldPos.x + (m_NodeDiameter * x);
			int checkY = worldPos.y + (m_NodeDiameter * y);

			// Check that the node is within gridWorld
			if (checkX >= m_StartingPoint.x && checkX < (m_StartingPoint.x + m_GridWorldSize.x) && checkY >= m_StartingPoint.y && checkY < (m_StartingPoint.y + m_GridWorldSize.y))
			{
				neighbours.push_back(&m_Nodes[{checkX, checkY}]);
			}
		}
	}

	return neighbours;
}