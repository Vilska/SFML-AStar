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

	// Clock for deltaTime
	sf::Clock deltaClock;

	// Window flags for "subwindows"
	ImGuiWindowFlags subWindowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;

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

			//Listen for two mouse clicks and set startNode and targetNode
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (index > 1)
					return;

				if (event.mouseButton.button != sf::Mouse::Button::Left)
					return;

				auto actualMousePosition = gui.GetViewportMousePosition({ event.mouseButton.x, event.mouseButton.y });
				Node& clickedNode = grid.NodeFromWorldPoint(actualMousePosition);

				if (!clickedNode.GetWalkable())
					return;

				clickedNode.GetShape().setFillColor(sf::Color::Cyan);
				startTargetNodes.push_back(clickedNode);

				index++;
			}
		});

		// Create dockspace lambda
		gui.CreateDockspace([&]()
		{		
			// Properties panel
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 10.0f));

			ImGui::Begin("Properties", (bool*)true, subWindowFlags);

			ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 6, 45));
			ImGui::PushTextWrapPos(200.0f);
			ImGui::TextWrapped("Select two nodes from the grid and the program will draw the shortest path between those two nodes using A* algorithm.");
			ImGui::PopTextWrapPos();

			ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 5, 150));
			if (ImGui::Button("Reset", ImVec2(128.0f, 32.0f)))
			{
				// Reset some of those vectors
				index = 0;
				grid.CreateGrid();
				grid.ClearPath();
				startTargetNodes.clear();
			}

			ImGui::End();
			ImGui::PopStyleVar();

			// Viewport
			ImGui::Begin("Viewport", (bool*)true, subWindowFlags);

			// Create viewport and draw nodes to it
			gui.CreateViewport([&](float scale, sf::RenderTexture& renderTexture)
			{
				for (auto& node : grid.GetNodes())
				{
					auto shape = node.second.GetShape();
					shape.setScale(scale, scale);
					shape.setPosition(shape.getPosition().x * scale, shape.getPosition().y * scale);
					renderTexture.draw(shape);
				}
			});

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

		if (index == 3)
		{
			for (auto& pathNode : grid.GetPath())
			{
				(*pathNode).GetShape().setFillColor(sf::Color::Magenta);
			}

			index++;
		}
	}

	gui.Shutdown();

	return 0;
}