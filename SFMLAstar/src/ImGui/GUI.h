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
	void Update(sf::Time deltaTime);
	void Render();
	void ProcessEvent(const sf::Event& event);

	void CreateDockspace(const std::function<void()>& func);
	void CreateViewport(const std::function<void(float scale, sf::RenderTexture& renderTexture)>& func);

	sf::Vector2f GetViewportMousePosition(const sf::Vector2i& mousePos);
private:
	sf::RenderWindow& m_Window;

	// Viewport specific stuff
	sf::RenderTexture m_RenderTexture;
	sf::Vector2f m_OriginalWindowSize = { 0, 0 };
	sf::Vector2f m_NewViewportSize = { 0, 0 };

	float m_PosY = 0;
	float m_OrgPosY = 0;
	float m_Scale = 1;
};
