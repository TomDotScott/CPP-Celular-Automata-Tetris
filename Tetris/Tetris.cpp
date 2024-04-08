#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>

#include "Game.h"
#include "Keyboard.h"
#include "Tetromino.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Cellular Automata Tetris");
	window.setFramerateLimit(60);

	auto* pixels = new sf::Uint8[static_cast<size_t>(SCREEN_WIDTH) * static_cast<size_t>(SCREEN_HEIGHT) * 4];

	sf::Texture texture;
	texture.create(SCREEN_WIDTH, SCREEN_HEIGHT);

	sf::Sprite sprite(texture);

	Game game;

	int updateCounter = 0;

	while (window.isOpen())
	{
		sf::Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		Keyboard::Update();

		if (++updateCounter == 4)
		{
			game.HandleInputs();
			game.Update();
			updateCounter = 0;
		}

		window.clear(sf::Color::Black);

		// Clear the pixel buffer
		memset(pixels, 0, static_cast<size_t>(SCREEN_WIDTH) * static_cast<size_t>(SCREEN_HEIGHT) * 4);

		game.Render(pixels);

		texture.update(pixels);

		window.draw(sprite);

		window.display();
	}

	return 0;
}
