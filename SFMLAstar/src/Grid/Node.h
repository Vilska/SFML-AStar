#pragma once

#include <SFML/Graphics.hpp>

class Node
{
public:
	Node() = default;
	Node(bool walkable, const sf::Vector2f& worldPosition, float diameter);

	inline sf::Vector2f GetWorldPosition() const { return m_WorldPosition; }
	inline bool GetWalkable() const { return m_Walkable; }
	inline int GetGCost() const { return m_gCost; }
	inline int GetHCost() const { return m_hCost; }
	inline int GetFCost() const { return m_gCost + m_hCost; }
	inline sf::RectangleShape GetShape() const { return m_Shape; }
	inline Node* GetParent() const { return m_Parent; }

	inline void SetGCost(int value) { m_gCost = value; }
	inline void SetHCost(int value) { m_hCost = value; }
	inline void SetParent(Node* node) { m_Parent = node; }

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
