#pragma once

#include <imgui.h>
#include <backends/imgui_impl_sfml.h>

#include "../Core/Window.h"

class GUI
{
public:
	GUI() = default;
	GUI(sf::RenderWindow& window);

	void Init();
	void Shutdown();
	void CreateDockspace(const std::function<void()>& func);
	void Update(sf::Time deltaTime);
	void Render();
	void ProcessEvent(const sf::Event& event);
private:
	sf::RenderWindow& m_Window;
};
