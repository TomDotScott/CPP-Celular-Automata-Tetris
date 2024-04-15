#pragma once
#include <SFML/Window/Event.hpp>

#include "GameGrid.h"
#include "Tetromino.h"


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
	GameGrid m_gameGrid;

	bool m_shouldMoveLeft;
	bool m_shouldMoveRight;
	bool m_shouldRotate;
	bool m_shouldSpeedUp;

	float m_movementTimer;

	void UpdateAutomata();
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
