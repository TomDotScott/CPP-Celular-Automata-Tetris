#pragma once
#include <array>
#include <SFML/Window/Keyboard.hpp>

class Keyboard
{
public:
	static void Update();

	// Has the button been pressed this frame?
	static bool IsButtonPressed(sf::Keyboard::Key key);

	// Is the button held down?
	static bool IsButtonDown(sf::Keyboard::Key key);

	// Has the button been released?
	static bool IsButtonReleased(sf::Keyboard::Key key);

private:
	// True the first frame the button is pressed
	static std::array<bool, sf::Keyboard::Key::KeyCount> m_pressedButtons;

	// True whilst the buttons are pressed
	static std::array<bool, sf::Keyboard::Key::KeyCount> m_downButtons;

	// True the frame the buttons are released
	static std::array<bool, sf::Keyboard::Key::KeyCount> m_releasedButtons;
};
