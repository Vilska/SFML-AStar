#include "Pathfinding.h"

// Constructor
Pathfinding::Pathfinding(Grid& grid)
	: m_Grid(grid) {}

// A* algorithm
void Pathfinding::FindPath(const sf::Vector2f& startPos, const sf::Vector2f& targetPos)
{
	// Start & target node
	Node& startNode = m_Grid.NodeFromWorldPoint(startPos);
	Node& targetNode = m_Grid.NodeFromWorldPoint(targetPos);

	// Vector for OPEN & CLOSED nodes
	std::vector<Node*> openSet;
	std::vector<Node*> closedSet;

	// Initialize startNode as OPEN
	openSet.push_back(&startNode);

	// Run this code block as long as OPEN vector contains nodes
	while (openSet.size() > 0)
	{
		// Set currentNode as openSets first node (placeholder)
		Node* currentNode = openSet[0];

		// Loop through the nodes which are set as OPEN
		for (int i = 1; i < openSet.size(); i++)
		{
			// If some of the nodes have smaller FCost or FCost is the same but distance to target is smaller, set that node as current
			if (openSet[i]->GetFCost() < currentNode->GetFCost() || openSet[i]->GetFCost() == currentNode->GetFCost() && openSet[i]->GetHCost() < currentNode->GetHCost())
				currentNode = openSet[i];
		}

		// Remove the current node from OPEN nodes and vice versa add it to CLOSE
		openSet.erase(std::remove(openSet.begin(), openSet.end(), currentNode), openSet.end());
		closedSet.push_back(currentNode);

		// Target found?
		if (*currentNode == targetNode)
		{
			RetracePath(startNode, targetNode);
			return;
		}

		// Scan through node's neighbours
		for (auto& neighbour : m_Grid.GetNeighbours(*currentNode))
		{
			// Ensure that neighbour is not an obstacle or CLOSED
			if (!neighbour->GetWalkable() || std::find(closedSet.begin(), closedSet.end(), neighbour) != closedSet.end())
				continue;

			// Calculate cost to neighbour node
			int movementCostToNeighbour = currentNode->GetGCost() + GetDistance(*currentNode, *neighbour);

			// Assign proper values to neighbour node
			neighbour->SetGCost(movementCostToNeighbour);
			neighbour->SetHCost(GetDistance(*neighbour, targetNode));
			neighbour->SetParent(currentNode);

			openSet.push_back(neighbour);
		}
	}
}

// Build path by checking each node's parent node, starting from the target node
void Pathfinding::RetracePath(const Node& startNode, Node& targetNode)
{
	// Declare vector for nodes
	std::vector<Node*> reversePath;

	// Hold pointer to avoid overwriting
	Node* currentNode = &targetNode;

	// As long as currentNode is not startNode add currentNode to vector and assign currentNode to node's parent
	while (!(*currentNode == startNode))
	{
		reversePath.push_back(currentNode);
		currentNode = currentNode->GetParent();
	}

	// Because the original path is "top-down" we have to reverse it
	std::vector<Node*> path;
	for (int i = reversePath.size() - 1; i > 0; i--)
	{
		path.push_back(reversePath[i]);
	}

	m_Grid.SetPath(path);
}

// Get distance between two nodes
int Pathfinding::GetDistance(const Node& nodeA, const Node& nodeB)
{
	// Calculate both nodes difference in both x and y axis as a absolute value
	int distanceX = abs(nodeA.GetWorldPosition().x - nodeB.GetWorldPosition().x) / m_Grid.GetNodeDiameter();
	int distanceY = abs(nodeA.GetWorldPosition().y - nodeB.GetWorldPosition().y) / m_Grid.GetNodeDiameter();

	// If distance in x axis is greater than in y axis make diagonal movement with y's difference and vice versa
	if (distanceX > distanceY)
		return (m_Grid.GetNodeDiagonal() * distanceY) + (m_Grid.GetNodeDiameter() * (distanceX - distanceY));

	return (m_Grid.GetNodeDiagonal() * distanceX) + (m_Grid.GetNodeDiameter() * (distanceY - distanceX));
}