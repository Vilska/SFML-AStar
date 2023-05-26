#include "GUI.h"

GUI::GUI(sf::RenderWindow& window)
	: m_Window(window)
{
	Init();
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
	ImGui::Begin("Dockspace demo", (bool*)true, window_flags);

	// Crate dockspace
	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

	func();

	// End main window
	ImGui::End();
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