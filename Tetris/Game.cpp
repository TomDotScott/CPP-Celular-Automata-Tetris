#include "Game.h"

#include <iostream>
#include <random>

#include "Keyboard.h"

Game::Game() :
	m_nextID(5),
	m_currentTetromino(nullptr),
	m_gameGrid{ },
	m_shouldMoveLeft(false),
	m_shouldMoveRight(false)
{
	m_currentTetromino = GenerateTetromino();
}

Game::~Game()
{
	delete m_currentTetromino;
}

void Game::HandleInputs()
{
	if (!m_currentTetromino)
	{
		return;
	}

	m_shouldMoveLeft = Keyboard::IsButtonDown(sf::Keyboard::Key::Left);
	m_shouldMoveRight = Keyboard::IsButtonDown(sf::Keyboard::Key::Right);
}

#if DEBUG_MOVEMENT
void Game::PrintGrid() const
{
	for (int gridY = 0; gridY < SCREEN_HEIGHT / GRID_SIZE; ++gridY)
	{
		for (int gridX = 0; gridX < SCREEN_WIDTH / GRID_SIZE; ++gridX)
		{
			if (m_gameGrid[gridY][gridX] > 0)
			{
				// std::cout << m_nextID << ",";
				printf("%x,", m_gameGrid[gridY][gridX]);
			}
			else
			{
				std::cout << 0 << ",";
			}
		}
		std::cout << "\n";
	}
}
#endif


void Game::Update()
{
	if (!m_currentTetromino)
	{
		m_currentTetromino = GenerateTetromino();
	}

	if (!MoveTetromino())
	{
		delete m_currentTetromino;
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
	for (int gridY = 0; gridY < SCREEN_HEIGHT / GRID_SIZE; ++gridY)
	{
		for (int gridX = 0; gridX < SCREEN_WIDTH / GRID_SIZE; ++gridX)
		{
			const uint32_t color = m_gameGrid[gridY][gridX];
			if (color > 0)
			{
				// Extract color channels
				const uint8_t red = color >> 24 & 0xFF;
				const uint8_t green = color >> 16 & 0xFF;
				const uint8_t blue = color >> 4 & 0xFF;
				const uint8_t alpha = color & 0xFF;

				const int pixelsIndex = (gridY * GRID_SIZE * SCREEN_WIDTH + gridX * GRID_SIZE) * 4;

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

Tetromino* Game::GenerateTetromino()
{
	Tetromino* tetromino = nullptr;

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

	m_nextID = RandomMultiple(1, 7);

	return tetromino;
}

void Game::RemoveCurrentTetrominoFromGrid()
{
	const sf::Vector2i gridPosition = m_currentTetromino->GetPosition() / GRID_SIZE;

	for (int blockY = 0; blockY < 2; ++blockY)
	{
		for (int blockX = 0; blockX < 4; ++blockX)
		{
			if (m_currentTetromino->GetBuffer()[blockY * 4 + blockX] & 0xFFFFFFFF && (gridPosition.y + blockY < SCREEN_HEIGHT / GRID_SIZE && gridPosition.x + blockX < SCREEN_WIDTH / GRID_SIZE))
			{
				m_gameGrid[gridPosition.y + blockY][gridPosition.x + blockX] = 0;
			}
		}
	}
}

void Game::AddTetrominoToGrid()
{
	const sf::Uint32* buffer = m_currentTetromino->GetBuffer();

	const sf::Vector2i gridPosition = m_currentTetromino->GetPosition() / GRID_SIZE;

	for (int blockY = 0; blockY < 2; ++blockY)
	{
		for (int blockX = 0; blockX < 4; ++blockX)
		{
			const sf::Uint32 block = buffer[blockY * 4 + blockX];

			if (block > 0 && (gridPosition.y + blockY < SCREEN_HEIGHT / GRID_SIZE && gridPosition.x + blockX < SCREEN_WIDTH / GRID_SIZE))
			{
				m_gameGrid[gridPosition.y + blockY][gridPosition.x + blockX] = block;
			}
		}
	}
}

bool Game::CanTetrominoMove()
{
	const sf::Uint32* buffer = m_currentTetromino->GetBuffer();

	sf::Vector2i nextGridPosition;

	if (m_shouldMoveLeft)
	{
		nextGridPosition = (m_currentTetromino->GetPosition() + sf::Vector2i(-GRID_SIZE, GRID_SIZE)) / GRID_SIZE;
	}
	else if(m_shouldMoveRight)
	{
		nextGridPosition = (m_currentTetromino->GetPosition() + sf::Vector2i(GRID_SIZE, GRID_SIZE)) / GRID_SIZE;
	}
	else
	{
		nextGridPosition = (m_currentTetromino->GetPosition() + sf::Vector2i(0, GRID_SIZE)) / GRID_SIZE;
	}

	for (int blockY = 0; blockY < 2; ++blockY)
	{
		for (int blockX = 0; blockX < 4; ++blockX)
		{
			const sf::Uint32 block = buffer[blockY * 4 + blockX];

			if (!(block & 0xFF))
			{
				continue;
			}

			if (nextGridPosition.y + blockY < SCREEN_HEIGHT / GRID_SIZE && nextGridPosition.x + blockX < SCREEN_WIDTH / GRID_SIZE)
			{
				if (m_gameGrid[nextGridPosition.y + blockY][nextGridPosition.x + blockX] & 0xFF)
				{
					return false;
				}
			}
		}
	}

	return true;
}

bool Game::MoveTetromino()
{
	if(m_currentTetromino == nullptr)
	{
		return false;
	}

	const bool bottomOfScreen = m_currentTetromino->GetPosition().y > SCREEN_HEIGHT - GRID_SIZE * 3;

	if (bottomOfScreen)
	{
		return false;
	}

#if DEBUG_MOVEMENT
	std::cout << "BEFORE MOVING:\n";
	PrintGrid();
#endif

	RemoveCurrentTetrominoFromGrid();

	if (!CanTetrominoMove())
	{
		AddTetrominoToGrid();
		return false;
	}

	if (m_shouldMoveLeft)
	{
		m_currentTetromino->MoveLeft(GRID_SIZE);
	}

	if(m_shouldMoveRight)
	{
		m_currentTetromino->MoveRight(GRID_SIZE);
	}

	m_currentTetromino->MoveDown(GRID_SIZE);

	AddTetrominoToGrid();

#if DEBUG_MOVEMENT
	std::cout << "AFTER MOVING:\n";
	PrintGrid();
#endif

	return true;
}
