#pragma once

#include <functional>

#include <SFML/Graphics.hpp>

class Window
{
public:
	Window() = default;
	Window(const sf::Vector2f& windowSize, const std::string& windowLabel);

	~Window();

	void Close();

	template<typename... Args>
	void Draw(Args&&... args)
	{
		m_Window->draw(std::forward<Args>(args)...);
	}

	void Update(const std::function<void()>& func);
	void CheckEvent(const std::function<void(const sf::Event)>& func);

	sf::RenderWindow& GetWindow() const { return *m_Window; }
private:
	sf::RenderWindow* m_Window;
};
