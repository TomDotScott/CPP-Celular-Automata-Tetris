#include "Keyboard.h"

void Keyboard::Update()
{
	std::array<bool, sf::Keyboard::Key::KeyCount> buttonsPressedThisFrame{};

	// Get the buttons that changed this frame
	for (int i = 0; i < sf::Keyboard::KeyCount; i++)
	{
		buttonsPressedThisFrame[i] = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i));

		// Is it newly pressed?
		m_pressedButtons[i] = buttonsPressedThisFrame[i] && !m_downButtons[i];

		// Is it newly released?
		m_releasedButtons[i] = !buttonsPressedThisFrame[i] && m_downButtons[i];

		// Store the state
		m_downButtons[i] = buttonsPressedThisFrame[i];
	}
}

bool Keyboard::IsButtonPressed(const sf::Keyboard::Key key)
{
	return m_pressedButtons[static_cast<int>(key)];
}

bool Keyboard::IsButtonDown(const sf::Keyboard::Key key)
{
	return m_downButtons[static_cast<int>(key)];
}

bool Keyboard::IsButtonReleased(const sf::Keyboard::Key key)
{
	return m_releasedButtons[static_cast<int>(key)];
}


std::array<bool, sf::Keyboard::Key::KeyCount> Keyboard::m_downButtons{};
std::array<bool, sf::Keyboard::Key::KeyCount> Keyboard::m_releasedButtons{};
std::array<bool, sf::Keyboard::Key::KeyCount> Keyboard::m_pressedButtons{};