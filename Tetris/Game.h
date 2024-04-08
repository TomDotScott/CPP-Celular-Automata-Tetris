#pragma once
#include <vector>
#include <SFML/Window/Event.hpp>

#include "Tetromino.h"

static constexpr unsigned SCREEN_WIDTH = 800;
static constexpr unsigned SCREEN_HEIGHT = 600;

static constexpr int GRID_SIZE = 10;


class Game
{
public:
	Game();
	~Game();
	void HandleInputs() const;
	void Update();
	void Render(uint8_t* pixels) const;

	static int RandomMultiple(int multiple, int max);
private:
	int m_nextID;
	Tetromino* m_currentTetromino;

	std::vector<Tetromino*> m_blocks;

	void BlitPixels(uint8_t* pixels) const;
	Tetromino* GenerateTetromino();
};
