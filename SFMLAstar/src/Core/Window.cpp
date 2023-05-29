#include "SFML/Window/Event.hpp"

#include "Window.h"

// Constructor
Window::Window(const sf::Vector2f& windowSize, const std::string& windowLabel)
{
	m_Window = new sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y), windowLabel);
}

// Deconstructor
Window::~Window()
{
	delete m_Window;
}

// Close
void Window::Close()
{
	m_Window->close();
}

// Update window
void Window::Update(const std::function<void()>& func)
{
	m_Window->clear(sf::Color::Black);
	func();
	m_Window->display();
}

// Check for events
void Window::CheckEvent(const std::function<void(const sf::Event)>& func)
{
	sf::Event event;

	while (m_Window->pollEvent(event))
	{
		func(event);
	}
}