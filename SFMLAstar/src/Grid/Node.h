#pragma once

#include <SFML/Graphics.hpp>

class Node
{
public:
	Node() = default;
	Node(bool walkable, const sf::Vector2f& worldPosition, float diameter);

	sf::Vector2f GetWorldPosition() const { return m_WorldPosition; }
	bool GetWalkable() const { return m_Walkable; }
	int GetGCost() const { return m_gCost; }
	int GetHCost() const { return m_hCost; }
	int GetFCost() const { return m_gCost + m_hCost; }

	sf::RectangleShape& GetShape() { return m_Shape; }
	const sf::RectangleShape& GetShape() const { return m_Shape; }

	Node* GetParent() const { return m_Parent; }

	void SetGCost(int value) { m_gCost = value; }
	void SetHCost(int value) { m_hCost = value; }
	void SetParent(Node* node) { m_Parent = node; }

	// == operator for checking two nodes equality
	bool operator==(const Node& node) const
	{
		return (m_WorldPosition == node.m_WorldPosition);
	}
private:
	// Variables
	bool m_Walkable;
	float m_Diameter;
	int m_gCost, m_hCost;

	sf::Vector2f m_WorldPosition;
	sf::RectangleShape m_Shape;
	Node* m_Parent;
};
