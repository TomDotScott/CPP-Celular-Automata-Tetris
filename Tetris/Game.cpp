#include "Game.h"

#include <iostream>
#include <random>

#include "Keyboard.h"

// TODO: Shouldn't be constant, should change over time... Until I think of an elegant way it is staying as a constexpr
static constexpr float MOVEMENT_TIME = 1 / 60.f;

Game::Game() :
	m_nextID(5),
	m_currentTetromino(nullptr),
	m_gameGrid{ },
	m_shouldMoveLeft(false),
	m_shouldMoveRight(false),
	m_shouldRotate(false),
	m_shouldSpeedUp(false),
	m_movementTimer(0.f)
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
	m_shouldRotate = Keyboard::IsButtonReleased(sf::Keyboard::Key::Up);
	m_shouldSpeedUp = Keyboard::IsButtonDown(sf::Keyboard::Key::Down);
}

#if DEBUG_MOVEMENT
void Game::PrintGrid() const
{
	for (int gridY = 0; gridY < SCREEN_HEIGHT / CELL_SIZE; ++gridY)
	{
		for (int gridX = 0; gridX < SCREEN_WIDTH / CELL_SIZE; ++gridX)
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


void Game::Update(const float deltaTime)
{
	if (m_currentTetromino)
	{
		RemoveCurrentTetrominoFromGrid();
	}

	sf::Uint32 nextGrid[SCREEN_HEIGHT / CELL_SIZE][SCREEN_WIDTH / CELL_SIZE]{};

	// Update the automata
	for (int gridY = 0; gridY < SCREEN_HEIGHT / CELL_SIZE; ++gridY)
	{
		for (int gridX = 0; gridX < SCREEN_WIDTH / CELL_SIZE; ++gridX)
		{
			const uint32_t color = m_gameGrid[gridY][gridX];

			if (color & 0xFF)
			{
				const uint32_t below = m_gameGrid[gridY + 1][gridX];
				const uint32_t left  = m_gameGrid[gridY + 1][gridX - 1];
				const uint32_t right = m_gameGrid[gridY + 1][gridX + 1];

				if (gridY == SCREEN_HEIGHT / CELL_SIZE - 1)
				{
					nextGrid[gridY][gridX] = color;
				}
				else if (gridY < SCREEN_HEIGHT / CELL_SIZE - 1 && below == 0)
				{
					nextGrid[gridY + 1][gridX] = color;
				}
				else if(gridX > 0 && left == 0)
				{
					nextGrid[gridY][gridX - 1] = color;
				}
				else if(gridX < SCREEN_WIDTH / CELL_SIZE - 1 && right == 0)
				{
					nextGrid[gridY][gridX + 1] = color;
				}
				else
				{
					nextGrid[gridY][gridX] = color;
				}
			}
		}
	}

	memcpy(m_gameGrid, nextGrid, SCREEN_HEIGHT / CELL_SIZE * SCREEN_WIDTH / CELL_SIZE * sizeof(uint32_t));

	if (m_currentTetromino)
	{
		AddTetrominoToGrid();
	}
	else
	{
		m_currentTetromino = GenerateTetromino();
		return;
	}

	float multiplier = 1.f;

	if(m_shouldSpeedUp)
	{
		multiplier = 4.f;
	}

	m_movementTimer += deltaTime * multiplier;

	if (m_shouldRotate /*&& CanTetrominoRotate()*/)
	{
		RemoveCurrentTetrominoFromGrid();

		m_currentTetromino->Rotate();

		AddTetrominoToGrid();
	}

	if (m_movementTimer > MOVEMENT_TIME)
	{
		if (!MoveTetromino())
		{
			delete m_currentTetromino;
			m_currentTetromino = GenerateTetromino();
		}

		m_movementTimer = 0.f;
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
	for (int gridY = 0; gridY < SCREEN_HEIGHT / CELL_SIZE; ++gridY)
	{
		for (int gridX = 0; gridX < SCREEN_WIDTH / CELL_SIZE; ++gridX)
		{
			const uint32_t color = m_gameGrid[gridY][gridX];
			if (color > 0)
			{
				// Extract color channels
				const uint8_t red = color >> 24 & 0xFF;
				const uint8_t green = color >> 16 & 0xFF;
				const uint8_t blue = color >> 4 & 0xFF;
				const uint8_t alpha = color & 0xFF;

				const int pixelsIndex = (gridY * CELL_SIZE * SCREEN_WIDTH + gridX * CELL_SIZE) * 4;

				// Blit the color to the pixels array for each pixel in the block
				for (int y = 0; y < CELL_SIZE; ++y)
				{
					for (int x = 0; x < CELL_SIZE; ++x)
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
	const sf::Vector2i gridPosition = m_currentTetromino->GetPosition() / CELL_SIZE;

	const Tetromino::Rotation* currentRotation = m_currentTetromino->GetShape();

	for (int blockY = 0; blockY < currentRotation->GetHeight(); ++blockY)
	{
		for (int blockX = 0; blockX < currentRotation->GetWidth(); ++blockX)
		{
			if (currentRotation->GetBuffer()[blockY * currentRotation->GetWidth() + blockX] & 0xFFFFFFFF && (gridPosition.y + blockY < SCREEN_HEIGHT / CELL_SIZE && gridPosition.x + blockX < SCREEN_WIDTH / CELL_SIZE))
			{
				// Adjust for block size
				for (int pixelY = 0; pixelY < BLOCK_SIZE; ++pixelY)
				{
					for (int pixelX = 0; pixelX < BLOCK_SIZE; ++pixelX)
					{
						m_gameGrid[gridPosition.y + blockY * BLOCK_SIZE + pixelY][gridPosition.x + blockX * BLOCK_SIZE + pixelX] = 0;
					}
				}
			}
		}
	}
}

void Game::AddTetrominoToGrid()
{
	const Tetromino::Rotation* rotation = m_currentTetromino->GetShape();
	const sf::Uint32* buffer = rotation->GetBuffer();

	const sf::Vector2i gridPosition = m_currentTetromino->GetPosition() / CELL_SIZE;

	for (int blockY = 0; blockY < rotation->GetHeight(); ++blockY)
	{
		for (int blockX = 0; blockX < rotation->GetWidth(); ++blockX)
		{
			const sf::Uint32 block = buffer[blockY * rotation->GetWidth() + blockX];

			if (block > 0 && (gridPosition.y + blockY < SCREEN_HEIGHT / CELL_SIZE && gridPosition.x + blockX < SCREEN_WIDTH / CELL_SIZE))
			{
				// Adjust for block size
				for (int pixelY = 0; pixelY < BLOCK_SIZE; ++pixelY)
				{
					for (int pixelX = 0; pixelX < BLOCK_SIZE; ++pixelX)
					{
						m_gameGrid[gridPosition.y + blockY * BLOCK_SIZE + pixelY][gridPosition.x + blockX * BLOCK_SIZE + pixelX] = block;
					}
				}
			}
		}
	}
}

bool Game::CanTetrominoMoveIntoSpace(const sf::Vector2i& newPosition) const
{
	const Tetromino::Rotation* rotation = m_currentTetromino->GetShape();
	const sf::Uint32* buffer = rotation->GetBuffer();

	for (int blockY = 0; blockY < rotation->GetHeight(); ++blockY)
	{
		for (int blockX = 0; blockX < rotation->GetWidth(); ++blockX)
		{
			const sf::Uint32 block = buffer[blockY * rotation->GetWidth() + blockX];

			if (!(block & 0xFF))
			{
				continue;
			}

			if (newPosition.y + blockY * BLOCK_SIZE < SCREEN_HEIGHT / CELL_SIZE && newPosition.x + blockX < SCREEN_WIDTH / CELL_SIZE)
			{
				for (int pixelY = 0; pixelY < BLOCK_SIZE; ++pixelY)
				{
					for (int pixelX = 0; pixelX < BLOCK_SIZE; ++pixelX)
					{
						if (m_gameGrid[newPosition.y + blockY * BLOCK_SIZE + pixelY][newPosition.x + blockX * BLOCK_SIZE + pixelX] & 0xFF)
						{
							return false;
						}
					}
				}
			}
		}
	}

	return true;
}

bool Game::CanTetrominoMoveDown() const
{
	return CanTetrominoMoveIntoSpace((m_currentTetromino->GetPosition() + sf::Vector2i(0, CELL_SIZE)) / CELL_SIZE);
}

bool Game::CanTetrominoMoveLeft() const
{
	return CanTetrominoMoveIntoSpace((m_currentTetromino->GetPosition() + sf::Vector2i(-CELL_SIZE, 0)) / CELL_SIZE);
}

bool Game::CanTetrominoMoveRight() const
{
	return CanTetrominoMoveIntoSpace((m_currentTetromino->GetPosition() + sf::Vector2i(CELL_SIZE, 0)) / CELL_SIZE);
}

bool Game::MoveTetromino()
{
	if (m_currentTetromino == nullptr)
	{
		return false;
	}

	const bool bottomOfScreen = m_currentTetromino->GetPosition().y > SCREEN_HEIGHT - CELL_SIZE * 3;

	if (bottomOfScreen)
	{
		return false;
	}

#if DEBUG_MOVEMENT
	std::cout << "BEFORE MOVING:\n";
	PrintGrid();
#endif

	RemoveCurrentTetrominoFromGrid();

	if (m_shouldMoveLeft && CanTetrominoMoveLeft())
	{
		m_currentTetromino->MoveLeft(CELL_SIZE);
	}

	if (m_shouldMoveRight && CanTetrominoMoveRight())
	{
		m_currentTetromino->MoveRight(CELL_SIZE);
	}

	if (!CanTetrominoMoveDown())
	{
		AddTetrominoToGrid();
		return false;
	}

	m_currentTetromino->MoveDown(CELL_SIZE);

	AddTetrominoToGrid();

#if DEBUG_MOVEMENT
	std::cout << "AFTER MOVING:\n";
	PrintGrid();
#endif

	return true;
}
