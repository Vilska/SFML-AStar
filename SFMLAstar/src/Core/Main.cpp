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
	Grid grid(sf::Vector2f(500, 100), sf::Vector2f(640, 640), 80);

	// Create pathfind
	Pathfinding pathfinding(grid);

	// startNode & targetNode
	int index = 0;
	std::vector<Node> startTargetNodes;

	// RenderTexture
	sf::RenderTexture renderTexture;
	renderTexture.create(1.0f, 1.0f);

	sf::Vector2f viewportSize(0.0f, 0.0f);
	sf::Vector2f orgViewportSize(1.0f, 1.0f);

	// Clock for deltaTime
	sf::Clock deltaClock;

	//float startIndex = 0;

	//sf::RectangleShape shape({640, 640});
	//shape.setPosition({ 500, 100 });

	float scale = 0;

	while (running)
	{
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

					float actualStartPosX = window.GetWindow().getSize().x - viewportSize.x;

					// Y MOUSE POSITION JATKA T�ST�
					sf::Vector2f actualMousePosition = { ((float)event.mouseButton.x - actualStartPosX) / scale, (float)event.mouseButton.y };

					std::cout << event.mouseButton.x << " " << event.mouseButton.y << std::endl;
					std::cout << actualMousePosition.x << " " << actualMousePosition.y << std::endl;

					Node& clickedNode = grid.NodeFromWorldPoint(actualMousePosition);

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

		//ImGui stuff
		gui.Update(deltaClock.restart());

		gui.CreateDockspace([&]()
		{
			// Properties panel
			ImGui::Begin("Properties");
			ImGui::Text("This is a properties panel!");

			ImGui::End();

			// Viewport
			ImGui::Begin("Testi");

			// TODO: ABSTRACT
			if (viewportSize.x == 0 && viewportSize.y == 0)
			{
				orgViewportSize = sf::Vector2f(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
				viewportSize = orgViewportSize;
				ImGui::End();
				return;
			}

			float scaleX = (viewportSize.x / orgViewportSize.x);
			float scaleY = (viewportSize.y / orgViewportSize.y);
			scale = std::min(scaleX, scaleY );

			renderTexture.create((orgViewportSize.x) * scale, (orgViewportSize.y) * scale);
			renderTexture.clear(sf::Color::Yellow);

			// JATKA T�ST� SKAALAAMISESSA ONGELMIA SCENEN ALKAESSA (EKA FRAME->)

			for (auto& node : grid.GetNodes())
			{
				auto& shape = node.second.GetShape();
				shape.setPosition(shape.getPosition().x * scale, shape.getPosition().y * scale);
				shape.setScale(scale, scale);
				renderTexture.draw(shape);
			}

			renderTexture.display();

			ImGui::Image(renderTexture);
			viewportSize = sf::Vector2f(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

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