#include <iostream>
#include <tuple>
#include <map>
#include <cmath>
#include <functional>
#include <vector>

#include <SFML/Graphics.hpp>

#include <imgui.h>
#include <backends/imgui_impl_sfml.h>

class Node
{
public:
	// Variables
	bool Walkable;
	sf::Vector2f WorldPosition;
	float Diameter;
	sf::RectangleShape Shape;

	int GCost;
	int HCost;

	Node* Parent = nullptr;

	// Constructors
	Node() = default;
	Node(bool walkable, const sf::Vector2f& worldPosition, float diameter)
		: Walkable(walkable), WorldPosition(worldPosition), Diameter(diameter)
	{
		// Set correct shape configurations
		Shape.setSize(sf::Vector2f(Diameter - 2, Diameter - 2));
		Shape.setPosition(sf::Vector2f(WorldPosition.x, WorldPosition.y));

		// If the Shape is not an obstacle and if it's paint it red
		if (Walkable)
			Shape.setFillColor(sf::Color::Green);
		else
			Shape.setFillColor(sf::Color::Red);
	}

	// Calculate the fCost of node
	inline int GetFCost() const { return GCost + HCost; }

	// == operator for checking two nodes equality
	bool operator==(const Node& node) const
	{
		return (WorldPosition == node.WorldPosition);
	}
};

class Grid
{
public:
	// Variables
	sf::Vector2f GridWorldSize;
	float NodeDiameter;
	int Diagonal;
	sf::Vector2f StartingPoint = { 1920 / 3, 1080 / 5 };

	std::map<std::tuple<float, float>, Node> Nodes;
	std::vector<Node*> Path;

	// Constructors
	Grid() = default;
	Grid(const sf::Vector2f& gridWorldSize, int nodeRadius)
		: GridWorldSize(gridWorldSize), NodeDiameter(nodeRadius * 2)
	{
		// Calculate diagonal distance using pythagoras
		Diagonal = sqrt((NodeDiameter * NodeDiameter) * 2);

		// Call grid creating function
		CreateGrid();
	}

	// Create the grid layout
	void CreateGrid()
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
				sf::Vector2f actualPos = { StartingPoint.x + (NodeDiameter * x), StartingPoint.y + (NodeDiameter * y) };

				// Check if value is either 0 or 1
				if (layout[(y * basis) + x] == 1) // Obstacle
					Nodes[{actualPos.x, actualPos.y}] = Node(false, actualPos, NodeDiameter);
				else
					Nodes[{actualPos.x, actualPos.y}] = Node(true, actualPos, NodeDiameter);
			}
		}
	}

	// Given world (window) point, return correct node based on its top-left corner coordinates
	Node& NodeFromWorldPoint(const sf::Vector2f& worldPosition)
	{
		int differenceX = (worldPosition.x - StartingPoint.x);
		int differenceY = (worldPosition.y - StartingPoint.y);
		int diffDividedX = differenceX / NodeDiameter;
		int diffDividedY = differenceY / NodeDiameter;

		if (differenceX >= NodeDiameter)
			differenceX -= NodeDiameter * diffDividedX;

		if (differenceY >= NodeDiameter)
			differenceY -= NodeDiameter * diffDividedY;

		int x = worldPosition.x - differenceX;
		int y = worldPosition.y - differenceY;

		return Nodes[{(float)x, (float)y}];
	}

	// Scan 3x3 area of given node to get its neighbours
	std::vector<Node*> GetNeighbours(const Node& node)
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
				int checkX = node.WorldPosition.x + (node.Diameter * x);
				int checkY = node.WorldPosition.y + (node.Diameter * y);

				// Check that the node is within gridWorld
				if (checkX >= StartingPoint.x && checkX < (StartingPoint.x + GridWorldSize.x) && checkY >= StartingPoint.y && checkY < (StartingPoint.y + GridWorldSize.y))
				{
					neighbours.push_back(&Nodes[{checkX, checkY}]);
				}
			}
		}

		return neighbours;
	}
};

class Pathfinding
{
public:
	// Variable for grid reference
	Grid& grid;

	// Constructors
	Pathfinding() = default;
	Pathfinding(Grid& _grid)
		: grid(_grid) {}

	// A* algorithm
	void FindPath(const sf::Vector2f& startPos, const sf::Vector2f& targetPos)
	{
		// Start & target node
		Node& startNode = grid.NodeFromWorldPoint(startPos);
		Node& targetNode = grid.NodeFromWorldPoint(targetPos);

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
				if (openSet[i]->GetFCost() < currentNode->GetFCost() || openSet[i]->GetFCost() == currentNode->GetFCost() && openSet[i]->HCost < currentNode->HCost)
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
			for (auto& neighbour : grid.GetNeighbours(*currentNode))
			{
				// Ensure that neighbour is not an obstacle or CLOSED
				if (!neighbour->Walkable || std::find(closedSet.begin(), closedSet.end(), neighbour) != closedSet.end())
					continue;

				// Calculate cost to neighbour node
				int movementCostToNeighbour = currentNode->GCost + GetDistance(*currentNode, *neighbour);

				// Assign proper values to neighbour node
				neighbour->GCost = movementCostToNeighbour;
				neighbour->HCost = GetDistance(*neighbour, targetNode);
				neighbour->Parent = currentNode;

				openSet.push_back(neighbour);
			}
		}
	}

	// Build path by checking each node's parent node, starting from the target node
	void RetracePath(const Node& startNode, Node& targetNode)
	{
		// Declare vector for nodes
		std::vector<Node*> reversePath;

		// Hold pointer to avoid overwriting
		Node* currentNode = &targetNode;

		// As long as currentNode is not startNode add currentNode to vector and assign currentNode to node's parent
		while (!(*currentNode == startNode))
		{
			reversePath.push_back(currentNode);
			currentNode = currentNode->Parent;
		}

		// Because the original path is "top-down" we have to reverse it
		std::vector<Node*> path;
		for (int i = reversePath.size() - 1; i > 0; i--)
		{
			path.push_back(reversePath[i]);
		}

		grid.Path = path;
	}

	// Get distance between two nodes
	int GetDistance(const Node& nodeA, const Node& nodeB)
	{
		// Calculate both nodes difference in both x and y axis as a absolute value
		int distanceX = abs(nodeA.WorldPosition.x - nodeB.WorldPosition.x) / grid.NodeDiameter;
		int distanceY = abs(nodeA.WorldPosition.y - nodeB.WorldPosition.y) / grid.NodeDiameter;

		// If distance in x axis is greater than in y axis make diagonal movement with y's difference and vice versa
		if (distanceX > distanceY)
			return (grid.Diagonal * distanceY) + (grid.NodeDiameter * (distanceX - distanceY));

		return (grid.Diagonal * distanceX) + (grid.NodeDiameter * (distanceY - distanceX));
	}
};

class Window
{
public:
	// Constructors
	Window() = default;
	Window(const sf::Vector2f& windowSize, const std::string& windowLabel)
	{
		m_Window = new sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y), windowLabel);
	}

	// Deconstructor
	~Window()
	{
		delete m_Window;
	}

	// Close
	void Close()
	{
		m_Window->close();
	}

	// Draw stuff
	template<typename... Args>
	void Draw(Args&&... args)
	{
		m_Window->draw(std::forward<Args>(args)...);
	}

	// Update window
	void Update(const std::function<void()>& func)
	{
		m_Window->clear(sf::Color::Black);
		func();
		m_Window->display();
	}

	// Check for events
	void CheckEvent(const std::function<void(const sf::Event)>& func)
	{
		sf::Event event;

		while (m_Window->pollEvent(event))
		{
			func(event);
		}
	}

	sf::RenderWindow& GetWindow() const { return *m_Window; }

private:
	sf::RenderWindow* m_Window;
};

class DearImGui
{
public:
	DearImGui(sf::RenderWindow& window)
		: m_Window(window) 
	{
		Init();
	}

	void Init()
	{
		ImGui::SFML::Init(m_Window);

		// Inform main ImGui that we are using docking
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}

	void Shutdown()
	{
		ImGui::SFML::Shutdown();
	}

	void CreateDockspace(const std::function<void()>& func)
	{
		// Main window flags
		ImGuiWindowFlags window_flags =
			ImGuiWindowFlags_MenuBar |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus;

		// Main window configs
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		// Create main window for dockspace
		ImGui::Begin("Dockspace demo", (bool*)true, window_flags);

		// Crate dockspace
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

		func();

		// End main window
		ImGui::End();
	}

	void Update(sf::Time deltaTime)
	{
		ImGui::SFML::Update(m_Window, deltaTime);
	}

	void Render()
	{
		ImGui::SFML::Render(m_Window);
	}

	void ProcessEvent(const sf::Event& event)
	{
		ImGui::SFML::ProcessEvent(m_Window, event);
	}
private:
	sf::RenderWindow& m_Window;
};

// Main function
int main(int argc, char** argv)
{
	// Declare running variable that indicates the state of program
	bool running = true;

	// Create window
	Window window(sf::Vector2f(1920, 1080), "A*");

	// Initialize ImGui
	DearImGui imgui(window.GetWindow());

	// Create grid
	Grid grid(sf::Vector2f(600, 600), 40);

	// Create pathfind
	Pathfinding pathfinding(grid);

	// startNode & targetNode
	int index = 0;
	std::vector<Node> startTargetNodes;

	// Clock for deltaTime
	sf::Clock deltaClock;

	while (running)
	{
		window.CheckEvent([&](const sf::Event& event)
		{
			// Pass events to ImGui
			imgui.ProcessEvent(event);

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

				sf::Vector2f mousePosition = { (float)event.mouseButton.x, (float)event.mouseButton.y };
				Node& clickedNode = grid.NodeFromWorldPoint(mousePosition);

				if (!clickedNode.Walkable)
					return;

				clickedNode.Shape.setFillColor(sf::Color::Cyan);
				startTargetNodes.push_back(clickedNode);

				index++;
			}
		});

		if (index == 3)
		{
			for (auto& pathNode : grid.Path)
			{
				(*pathNode).Shape.setFillColor(sf::Color::Magenta);
			}

			index++;
		}


		// Viewport as texture
		static ImVec2 viewportSize{ 500, 500 };
		sf::RenderTexture renderTexture{};
		renderTexture.create(viewportSize.x, viewportSize.y);
		renderTexture.clear(sf::Color::Black);

		for (auto& pair : grid.Nodes)
		{
			Node& node = pair.second;

			renderTexture.draw(node.Shape);
		}

		// ImGui stuff
		imgui.Update(deltaClock.restart());

		imgui.CreateDockspace([&]()
		{
			ImGui::Begin("Testi");
			
			viewportSize = ImGui::GetWindowSize();
			ImGui::Image(renderTexture);

			ImGui::End();
		});

		window.Update([&]() 
		{
			for (auto& pair : grid.Nodes)
			{
				Node& node = pair.second;

				// Draw shapes
				//window.Draw(node.Shape);

				// Draw ImGui
				imgui.Render();
			}
		});

		// Find path when both startNode and targetNode are specified
		if (index == 2)
		{
			pathfinding.FindPath(startTargetNodes[0].WorldPosition, startTargetNodes[1].WorldPosition);
			index++;
		}
	}

	imgui.Shutdown();

	return 0;
}