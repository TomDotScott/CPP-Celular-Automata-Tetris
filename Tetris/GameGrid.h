#pragma once
#include <array>
#include <vector>

static constexpr unsigned SCREEN_WIDTH = 400;
static constexpr unsigned SCREEN_HEIGHT = 800;

static constexpr uint32_t INVALID_CELL = 0x69696969;
static constexpr int CELL_SIZE = 4;

static constexpr unsigned NUM_ROWS = SCREEN_HEIGHT / CELL_SIZE;
static constexpr unsigned NUM_COLS = SCREEN_WIDTH / CELL_SIZE;

class GameGrid
{
public:
	GameGrid();
	void CheckForCompletion();
	uint32_t GetValue(unsigned row, unsigned col) const;
	void SetNextState(const std::array<std::array<uint32_t, NUM_COLS>, NUM_ROWS>& nextState);
	void ClearValue(unsigned row, unsigned col);
	void SetValue(unsigned row, unsigned col, uint32_t value);


private:
	std::array<std::array<uint32_t, NUM_COLS>, NUM_ROWS> m_grid;

	std::vector<std::pair<int, int>> FindPath(uint32_t valueToLookFor, unsigned row, unsigned col, std::array<std::array<bool, NUM_COLS>, NUM_ROWS>& visited);
	bool FindPath(uint32_t valueToLookFor, unsigned row, unsigned col, std::vector<std::pair<int, int>>& path, std::array<std::array<bool, NUM_COLS>, NUM_ROWS>& visited);
	bool IsValid(unsigned row, unsigned col) const;
};
