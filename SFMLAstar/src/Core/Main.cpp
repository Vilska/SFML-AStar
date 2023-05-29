#include "Window.h"
#include "../ImGui/GUI.h"
#include "../Grid/Pathfinding.h"

#include <iostream>

// Main function
int main(int argc, char** argv)
{
	// Declare running variable that indicates the state of program
	bool running = true;

	// Create window
	Window window(sf::Vector2f(1920, 1080), "A*");

	// Initialize ImGui
	GUI gui(window.GetWindow());

	// Create grid
	Grid grid(sf::Vector2f(600, 200), sf::Vector2f(640, 640), 80);

	// Create pathfind
	Pathfinding pathfinding(grid);

	// startNode & targetNode
	int index = 0;
	std::vector<Node> startTargetNodes;

	// RenderTexture
	sf::RenderTexture renderTexture;
	renderTexture.create(1.0f, 1.0f);

	// Clock for deltaTime
	sf::Clock deltaClock;

	// TESTI

	sf::Vector2f originalWindowSize = { (float)window.GetWindow().getSize().x, (float)window.GetWindow().getSize().y };
	sf::Vector2f newViewportSize = { 0, 0 };

	float scale = 0;

	float posY = 0;
	float orgPosY = 0;

	while (running)
	{
		// Update ImGui
		gui.Update(deltaClock.restart());

		window.CheckEvent([&](const sf::Event& event)
		{
			// Pass events to ImGui
			gui.ProcessEvent(event);

			// Close window when it is supposed to going to be closed
			if (event.type == sf::Event::Closed)
			{
				window.Close();
				running = false;
			}

			// Listen for two mouse clicks and set startNode and targetNode
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (index > 1)
					return;

				if (event.mouseButton.button != sf::Mouse::Button::Left)
					return;

				float actualStartPosX = (window.GetWindow().getSize().x - newViewportSize.x);
				float actualStatPosY = posY / orgPosY;

				sf::Vector2f actualMousePosition = { ((float)event.mouseButton.x - actualStartPosX) / scale, ((float)event.mouseButton.y - 35) / actualStatPosY}; // 35 offset on Y 
				Node& clickedNode = grid.NodeFromWorldPoint(actualMousePosition);

				std::cout << event.mouseButton.x << " " << event.mouseButton.y << std::endl;
				std::cout << actualMousePosition.x << " " << actualMousePosition.y << std::endl;

				if (!clickedNode.GetWalkable())
					return;

				clickedNode.GetShape().setFillColor(sf::Color::Cyan);
				startTargetNodes.push_back(clickedNode);

				//index++;
			}
		});

		if (index == 3)
		{
			for (auto& pathNode : grid.GetPath())
			{
				(*pathNode).GetShape().setFillColor(sf::Color::Magenta);
			}

			index++;
		}

		gui.CreateDockspace([&]()
		{
			// Properties panel
			ImGui::Begin("Properties");
			ImGui::Text("This is a properties panel!");

			ImGui::End();

			// Viewport
			ImGui::Begin("Testi");

			// Assign window size to variable for later calculations
			sf::Vector2f windowSize = *(sf::Vector2f*)&window.GetWindow().getSize();
			newViewportSize = *(sf::Vector2f*)&ImGui::GetWindowSize();

			// Calculate scale values for correct scaling & positioning
			float scaleX = newViewportSize.x / originalWindowSize.x;
			float scaleY = newViewportSize.y / originalWindowSize.y;
			scale = std::min(scaleX, scaleY);

			renderTexture.create(newViewportSize.x, newViewportSize.y);

			// Iterate through nodes and draw them dynamically to correct position
			for (auto& node : grid.GetNodes())
			{
				auto shapeX = node.second.GetShape();
				shapeX.setScale(scale, scale);
				shapeX.setPosition(shapeX.getPosition().x * scale, shapeX.getPosition().y * scale);
				renderTexture.draw(shapeX);
			}

			// JÄNIS
			sf::RectangleShape shape;
			shape.setPosition(600, 200);
			shape.setSize({ 640, 640 });
			shape.setFillColor(sf::Color::Transparent);

			static int counter = 0;
			if (counter == 0)
			{
				orgPosY = shape.getPosition().y;
				posY = orgPosY;
				counter++;
			}

			shape.setScale(scale, scale);
			shape.setPosition(shape.getPosition().x* scale, shape.getPosition().y* scale);

			posY = shape.getPosition().y;

			renderTexture.draw(shape);

			// Display render texture
			renderTexture.display();

			ImGui::Image(renderTexture);

			ImGui::End();
		});

		// Window update
		window.Update([&]()
		{
			// Draw ImGui
			gui.Render();
		});


		// Find path when both startNode and targetNode are specified
		if (index == 2)
		{
			pathfinding.FindPath(startTargetNodes[0].GetWorldPosition(), startTargetNodes[1].GetWorldPosition());
			index++;
		}
	}

	gui.Shutdown();

	return 0;
}