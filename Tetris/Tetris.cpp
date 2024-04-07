#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>

#include "Tetromino.h"

int RandomMultiple(const int multiple, const int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, max / multiple);

	return dist(gen) * multiple;
}

void BlitPixels(uint8_t* pixels, const uint32_t* block, const sf::Vector2i& position, const int pixelsWidth)
{
	// Loop through each pixel block in the block
	for (int blockY = 0; blockY < 2; ++blockY)
	{
		for (int blockX = 0; blockX < 4; ++blockX)
		{
			constexpr int blockSize = 10;
			const int blockIndex = blockY * 4 + blockX;

			const int pixelsIndex = ((position.y + blockY * blockSize) * pixelsWidth + position.x + blockX * blockSize) * 4;

			const uint32_t color = block[blockIndex];

			// Extract color channels
			const uint8_t red = color >> 24 & 0xFF;
			const uint8_t green = color >> 16 & 0xFF;
			const uint8_t blue = color >> 4 & 0xFF;
			const uint8_t alpha = color & 0xFF;

			if (color > 0)
			{
				// Blit the color to the pixels array for each pixel in the block
				for (int y = 0; y < blockSize; ++y)
				{
					for (int x = 0; x < blockSize; ++x)
					{
						const int pixelIndex = pixelsIndex + (y * pixelsWidth + x) * 4;
						pixels[pixelIndex] = red;
						pixels[pixelIndex + 1] = green;
						pixels[pixelIndex + 2] = blue;
						pixels[pixelIndex + 3] = alpha;
					}
				}
			}
		}
	}
}

void GenerateTetrominoes(std::vector<Tetromino*>& tetrominoes)
{
	for (int i = 0; i < 49; ++i)
	{
		switch (i % 7)
		{
		case 0:
			tetrominoes.emplace_back(new LBlock());
			break;
		case 1:
			tetrominoes.emplace_back(new JBlock());
			break;
		case 2:
			tetrominoes.emplace_back(new IBlock());
			break;
		case 3:
			tetrominoes.emplace_back(new SBlock());
			break;
		case 4:
			tetrominoes.emplace_back(new ZBlock());
			break;
		case 5:
			tetrominoes.emplace_back(new TBlock());
			break;
		case 6:
			tetrominoes.emplace_back(new OBlock());
			break;
		}
	}
}


int main()
{
	static constexpr unsigned SCREEN_WIDTH = 800;
	static constexpr unsigned SCREEN_HEIGHT = 600;

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Cellular Automata Tetris");
	window.setFramerateLimit(60);

	auto* pixels = new sf::Uint8[static_cast<size_t>(SCREEN_WIDTH) * static_cast<size_t>(SCREEN_HEIGHT) * 4];

	sf::Texture texture;
	texture.create(SCREEN_WIDTH, SCREEN_HEIGHT);

	sf::Sprite sprite(texture);

	std::vector<Tetromino*> blocks;

	GenerateTetrominoes(blocks);

	for (auto* block : blocks)
	{
		const int randomX = RandomMultiple(10, 400);
		const int randomY = RandomMultiple(10, 400);

		block->SetPosition({ randomX, randomY});
	}

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

		// Clear the pixel buffer
		memset(pixels, 0, static_cast<size_t>(SCREEN_WIDTH) * static_cast<size_t>(SCREEN_HEIGHT) * 4);

		// Update the pixels
		for (auto& block : blocks)
		{
			BlitPixels(pixels, block->GetBuffer(), block->GetPosition(), SCREEN_WIDTH);
		}

		texture.update(pixels);

		window.draw(sprite);

		window.display();
	}

	return 0;
}
