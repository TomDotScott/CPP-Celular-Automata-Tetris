#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Cellular Automata Tetris");

	static constexpr unsigned W = 800;
	static constexpr unsigned H = 600;

	sf::Uint8* pixels = new sf::Uint8[W * H * sizeof(unsigned)];

	sf::Texture texture;
	texture.create(W, H);

	for (register int i = 0; i < (W * H * 4); i += 4)
	{
		pixels[i] = 255;
		pixels[i + 1] = 0;
		pixels[i + 2] = 0;
		pixels[i + 3] = 255;
	}

	texture.update(pixels);

	sf::Sprite sprite(texture);

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

		window.clear(sf::Color::Black);

		window.draw(sprite);

		window.display();
	}

	return 0;
}