#include <iostream>
#include <tuple>
#include <map>

#include <SFML/Graphics.hpp>

struct Node
{
	bool Walkable;
	sf::Vector2f WorldPosition;
	float Diameter;
	sf::RectangleShape Shape;

	Node() = default;
	Node(bool walkable, const sf::Vector2f& worldPosition, float diameter)
		: Walkable(walkable), WorldPosition(worldPosition), Diameter(diameter)
	{
		Shape.setSize(sf::Vector2f(Diameter - 2, Diameter - 2));
		Shape.setFillColor(sf::Color::Magenta);
		Shape.setPosition(sf::Vector2f(worldPosition.x, worldPosition.y));
	}
};

class Grid
{
public:
	sf::Vector2f GridWorldSize;
	float NodeDiameter;

	float GridSizeX, GridSizeY;

	std::map<std::tuple<float, float>, Node> Nodes;

	Grid() = default;
	Grid(const sf::Vector2f& gridWorldSize, int nodeRadius)
		: GridWorldSize(gridWorldSize), NodeDiameter(nodeRadius * 2) 
	{
		GridSizeX = gridWorldSize.x / NodeDiameter;
		GridSizeY = gridWorldSize.y / NodeDiameter;
		CreateGrid();
	}

	void CreateGrid()
	{
		sf::Vector2f startingPoint = { 1920 / 3, 1080 / 5};
		for (int i = 0; i < GridSizeX; i++)
		{
			for (int j = 0; j < GridSizeY; j++)
			{
				sf::Vector2f actualPos = { startingPoint.x + (NodeDiameter * i), startingPoint.y + (NodeDiameter * j) };
				Nodes[{actualPos.x, actualPos.y}] = Node(true, actualPos, NodeDiameter);
			}
		}
	}

	Node NodeFromWorldPoint(const sf::Vector2f worldPosition)
	{
		// TODO
	}
};

int main(int argc, char** argv)
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "A*");

	Grid grid(sf::Vector2f(600, 600), 40);

	while (window.isOpen())
	{
		sf::Event event;
		
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button != sf::Mouse::Button::Left)
					continue;
				
				sf::Vector2f mousePosition = { (float)event.mouseButton.x, (float)event.mouseButton.y };
				std::cout << mousePosition.x << " " << mousePosition.y << std::endl;
			}
		}

		window.clear(sf::Color::Black);

		for (auto pair : grid.Nodes)
		{
			const std::tuple<float, float>& key = pair.first;
			const Node& node = pair.second;

			window.draw(node.Shape);
		}

		window.display();
	}

	return 0;
}