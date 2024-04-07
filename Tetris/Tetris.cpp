#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>

#include "Game.h"
#include "Tetromino.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Cellular Automata Tetris");
	window.setFramerateLimit(10);

	auto* pixels = new sf::Uint8[static_cast<size_t>(SCREEN_WIDTH) * static_cast<size_t>(SCREEN_HEIGHT) * 4];

	sf::Texture texture;
	texture.create(SCREEN_WIDTH, SCREEN_HEIGHT);

	sf::Sprite sprite(texture);

	Game game;


	while (window.isOpen())
	{
		sf::Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if(event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
			{
				game.HandleInputs(event);
			}
		}

		game.Update();
		

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
