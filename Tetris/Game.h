#pragma once
#include <SFML/Window/Event.hpp>

#include "Tetromino.h"

static constexpr unsigned SCREEN_WIDTH = 800;
static constexpr unsigned SCREEN_HEIGHT = 600;

static constexpr int CELL_SIZE = 2;
static constexpr int BLOCK_SIZE = 8;

#define DEBUG_MOVEMENT 0

class Game
{
public:
	Game();
	~Game();
	void HandleInputs();

	void Update(float deltaTime);
	void Render(uint8_t* pixels) const;

	static int RandomMultiple(int multiple, int max);
private:
	int m_nextID;
	Tetromino* m_currentTetromino;
	sf::Uint32 m_gameGrid[SCREEN_HEIGHT / CELL_SIZE][SCREEN_WIDTH / CELL_SIZE];

	bool m_shouldMoveLeft;
	bool m_shouldMoveRight;
	bool m_shouldRotate;
	bool m_shouldSpeedUp;

	float m_movementTimer;

	void BlitPixels(uint8_t* pixels) const;
	Tetromino* GenerateTetromino();
	void RemoveCurrentTetrominoFromGrid();
	void AddTetrominoToGrid();
	bool CanTetrominoMoveIntoSpace(const sf::Vector2i& newPosition) const;
	bool CanTetrominoMoveDown() const;
	bool CanTetrominoMoveLeft() const;
	bool CanTetrominoMoveRight() const;
	bool MoveTetromino();

#if DEBUG_MOVEMENT
	void PrintGrid() const;
#endif
};
