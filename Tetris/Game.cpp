#include "Game.h"

#include <iostream>
#include <random>

#include "Keyboard.h"

Game::Game() :
	m_nextID(0),
	m_currentTetromino(nullptr)
{
	m_currentTetromino = GenerateTetromino();
}

Game::~Game()
{
	for (const auto* tetromino : m_blocks)
	{
		delete tetromino;
	}
}

void Game::HandleInputs() const
{
	if(!m_currentTetromino)
	{
		return;
	}

	if (Keyboard::IsButtonDown(sf::Keyboard::Key::Left))
	{
		m_currentTetromino->MoveLeft(GRID_SIZE);
	}

	if (Keyboard::IsButtonDown(sf::Keyboard::Key::Right))
	{
		m_currentTetromino->MoveRight(GRID_SIZE);
	}
}

void Game::Update()
{
	if (!m_currentTetromino)
	{
		return;
	}

	std::cout << m_currentTetromino->GetPosition().y << std::endl;

	if (m_currentTetromino->GetPosition().y + (GRID_SIZE * 2) < SCREEN_HEIGHT)
	{
		m_currentTetromino->MoveDown(GRID_SIZE);
	}
	else
	{
		m_currentTetromino = GenerateTetromino();
	}
}

void Game::Render(uint8_t* pixels) const
{
	BlitPixels(pixels);
}

int Game::RandomMultiple(const int multiple, const int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, max / multiple);

	return dist(gen) * multiple;
}

void Game::BlitPixels(uint8_t* pixels) const
{
	for (const auto* tetromino : m_blocks)
	{
		// const uint32_t* block, const sf::Vector2i& position, const int pixelsWidth
		// Loop through each pixel block in the block
		for (int blockY = 0; blockY < 2; ++blockY)
		{
			for (int blockX = 0; blockX < 4; ++blockX)
			{
				const int blockIndex = blockY * 4 + blockX;

				const int pixelsIndex = ((tetromino->GetPosition().y + blockY * GRID_SIZE) * SCREEN_WIDTH + tetromino->GetPosition().x + blockX * GRID_SIZE) * 4;

				const uint32_t color = tetromino->GetBuffer()[blockIndex];

				// Extract color channels
				const uint8_t red = color >> 24 & 0xFF;
				const uint8_t green = color >> 16 & 0xFF;
				const uint8_t blue = color >> 4 & 0xFF;
				const uint8_t alpha = color & 0xFF;

				if (color > 0)
				{
					// Blit the color to the pixels array for each pixel in the block
					for (int y = 0; y < GRID_SIZE; ++y)
					{
						for (int x = 0; x < GRID_SIZE; ++x)
						{
							const int pixelIndex = pixelsIndex + (y * SCREEN_WIDTH + x) * 4;
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
}

Tetromino* Game::GenerateTetromino()
{
	Tetromino* tetromino;
	switch (m_nextID)
	{
	case 0:
		tetromino = new LBlock();
		break;
	case 1:
		tetromino = new JBlock();
		break;
	case 2:
		tetromino = new IBlock();
		break;
	case 3:
		tetromino = new SBlock();
		break;
	case 4:
		tetromino = new ZBlock();
		break;
	case 5:
		tetromino = new TBlock();
		break;
	case 6:
		tetromino = new OBlock();
		break;
	}

	m_blocks.emplace_back(tetromino);
	m_nextID = RandomMultiple(1, 7);

	return tetromino;
}
