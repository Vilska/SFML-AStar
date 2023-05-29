#pragma once

#include "Node.h"

#include <SFML/Graphics.hpp>

class Grid
{
public:
	Grid() = default;
	Grid(const sf::Vector2f& startingPoint, const sf::Vector2f& gridWorldSize, int diameter);

	void CreateGrid();
	Node& NodeFromWorldPoint(const sf::Vector2f& worldPosition);

	int GetNodeDiameter() const { return m_NodeDiameter; }
	int GetNodeDiagonal() const { return m_Diagonal; }
	std::vector<Node*> GetNeighbours(const Node& node);
	std::vector<Node*> GetPath() const { return m_Path; }

	const std::map<std::tuple<float, float>, Node>& GetNodes() const { return m_Nodes; }
	std::map<std::tuple<float, float>, Node>& GetNodes() { return m_Nodes; }

	void SetPath(std::vector<Node*>& nodes) { m_Path = nodes; }
private:
	int m_Diagonal;
	int m_NodeDiameter;

	sf::Vector2f m_GridWorldSize;
	sf::Vector2f m_StartingPoint;

	std::map<std::tuple<float, float>, Node> m_Nodes;
	std::vector<Node*> m_Path;
};
