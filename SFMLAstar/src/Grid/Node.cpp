#include "Node.h"

Node::Node(bool walkable, const sf::Vector2f& worldPosition, float diameter)
	: m_Walkable(walkable), m_WorldPosition(worldPosition), m_Diameter(diameter) 
{
	// Set correct shape configurations
	m_Shape.setSize(sf::Vector2f(m_Diameter, m_Diameter));
	m_Shape.setPosition(sf::Vector2f(m_WorldPosition.x, m_WorldPosition.y));

	// If the Shape is not an obstacle and if it's paint it red
	if (m_Walkable)
		m_Shape.setFillColor(sf::Color::Green);
	else
		m_Shape.setFillColor(sf::Color::Red);
}