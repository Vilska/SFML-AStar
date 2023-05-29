#include "GUI.h"

// Constructor
GUI::GUI(sf::RenderWindow& window)
	: m_Window(window)
{
	Init();
	m_OriginalWindowSize = { (float)m_Window.getSize().x, (float)m_Window.getSize().y };
}

// Initialize ImGui
void GUI::Init()
{
	// Initialize
	ImGui::SFML::Init(m_Window);

	// Inform main ImGui that we are using docking
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

// Handle shutdown
void GUI::Shutdown()
{
	ImGui::SFML::Shutdown();
}

// Handle updating
void GUI::Update(sf::Time deltaTime)
{
	ImGui::SFML::Update(m_Window, deltaTime);
}

// Handle rendering
void GUI::Render()
{
	ImGui::SFML::Render(m_Window);
}

// Handle event processing
void GUI::ProcessEvent(const sf::Event& event)
{
	ImGui::SFML::ProcessEvent(m_Window, event);
}

// Handle dockspace creation
void GUI::CreateDockspace(const std::function<void()>& func)
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
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	ImGui::Begin("Dockspace demo", (bool*)true, window_flags);

	// Crate dockspace
	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_AutoHideTabBar);

	func();

	// End main window
	ImGui::End();
	ImGui::PopStyleVar();
}

// Handle viewport creation
void GUI::CreateViewport(const std::function<void(float scale, sf::RenderTexture& renderTexture)>& func)
{
	// Assign window size and viewport size
	sf::Vector2f windowSize = { (float)m_Window.getSize().x, (float)m_Window.getSize().y };
	m_NewViewportSize = *(sf::Vector2f*)&ImGui::GetWindowSize();

	// Calculate correct scale values for dynamic scaling & positioning
	float scaleX = m_NewViewportSize.x / m_OriginalWindowSize.x;
	float scaleY = m_NewViewportSize.y / m_OriginalWindowSize.y;
	m_Scale = std::min(scaleX, scaleY);

	// Create render texture and assign a size for it
	m_RenderTexture.create(m_NewViewportSize.x, m_NewViewportSize.y);

	// Run function given in app (drawing) and pass scale value and render texture along
	func(m_Scale, m_RenderTexture);

	// This shape is some kind of a trick up my sleeve, with it's Y value we can calculate correct Y position between viewport and window
	sf::RectangleShape shape;
	shape.setPosition(600, 200);
	shape.setSize({ 100, 100 });
	shape.setFillColor(sf::Color::Transparent);

	// Assign original trick shape Y position
	static int counter = 0;
	if (counter == 0)
	{
		m_OrgPosY = shape.getPosition().y;
		m_PosY = m_OrgPosY;
		counter++;
	}

	// Scale & transform trick shape
	shape.setScale(m_Scale, m_Scale);
	shape.setPosition(shape.getPosition().x * m_Scale, shape.getPosition().y * m_Scale);

	// Assign new trick shape's Y position
	m_PosY = shape.getPosition().y;

	// Draw trick shape
	m_RenderTexture.draw(shape);

	// Display render texture
	m_RenderTexture.display();

	// Pass render texture to ImGui
	ImGui::Image(m_RenderTexture);
}

// Get mouse coordinates from viewport that matches the window ones
sf::Vector2f GUI::GetViewportMousePosition(const sf::Vector2i& mousePos)
{
	float actualStartPosX = (m_Window.getSize().x - m_NewViewportSize.x);
	float actualStatPosY = m_PosY / m_OrgPosY;

	sf::Vector2f actualMousePosition = { ((float)mousePos.x - actualStartPosX) / m_Scale, ((float)mousePos.y - 15) / actualStatPosY}; // 35 offset on Y
	return actualMousePosition;
}