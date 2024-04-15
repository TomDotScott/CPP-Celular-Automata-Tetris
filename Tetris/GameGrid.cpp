#include "GameGrid.h"

#include <utility>
#include <vector>

GameGrid::GameGrid() :
	m_grid()
{
}

void GameGrid::CheckForCompletion()
{
	std::vector<std::pair<int, int>> cellsToCheck;
	for (int i = 0; i < m_grid.size(); ++i)
	{
		if (m_grid[i][0] & 0xFF)
		{
			cellsToCheck.emplace_back(std::make_pair(i, 0));
		}
	}

	if (cellsToCheck.empty())
	{
		return;
	}

	std::array<std::array<bool, NUM_COLS>, NUM_ROWS> visitedCells{};

	for (const auto& indices : cellsToCheck)
	{
		const int row = indices.first;
		const int col = indices.second;

		const uint32_t colour = m_grid[row][col];
		const std::vector<std::pair<int, int>> cellsToRemove = FindPath(colour, row, col, visitedCells);

		if (cellsToRemove.empty())
		{
			continue;
		}

		for (const auto& removeIndices : cellsToRemove)
		{
			bool removing = true;
			int y = removeIndices.first;

			while (removing)
			{
				m_grid[y][removeIndices.second] = 0;
				// m_grid[y][removeIndices.second] = 0x39FF14FF;


				removing = IsValid(++y, removeIndices.second) && m_grid[y][removeIndices.second] == colour;
			}

			removing = false;
			while (removing)
			{
				m_grid[y][removeIndices.second] = 0;

				removing = IsValid(--y, removeIndices.second) && m_grid[y][removeIndices.second] == colour;
			}
		}
	}
}

uint32_t GameGrid::GetValue(const unsigned row, const unsigned col) const
{
	if (IsValid(row, col))
	{
		return m_grid[row][col];
	}

	return INVALID_CELL;
}

std::vector<std::pair<int, int>> GameGrid::FindPath(const uint32_t valueToLookFor, unsigned row, unsigned col, std::array<std::array<bool, NUM_COLS>, NUM_ROWS>& visited)
{
	std::vector<std::pair<int, int>> path;

	FindPath(valueToLookFor, row, col, path, visited);

	return path;
}

void GameGrid::SetNextState(const std::array<std::array<uint32_t, NUM_COLS>, NUM_ROWS>& nextState)
{
	m_grid = nextState;
}

void GameGrid::ClearValue(const unsigned row, const unsigned col)
{
	if (!IsValid(row, col))
	{
		return;
	}

	m_grid[row][col] = 0;
}

void GameGrid::SetValue(const unsigned row, const unsigned col, const uint32_t value)
{
	if (!IsValid(row, col))
	{
		return;
	}

	m_grid[row][col] = value;
}

bool GameGrid::IsValid(const unsigned row, const unsigned col) const
{
	return row < m_grid.size() && col < m_grid[0].size();
}

bool GameGrid::FindPath(const uint32_t valueToLookFor, unsigned row, unsigned col, std::vector<std::pair<int, int>>& path, std::array<std::array<bool, NUM_COLS>, NUM_ROWS>& visited)
{
	if (!IsValid(row, col) || visited[row][col] || m_grid[row][col] != valueToLookFor)
	{
		return false;
	}

	path.emplace_back(row, col);
	visited[row][col] = true;

	// Base case: If the current cell contains the value to look for, and it's the destination cell
	if (m_grid[row][col] == valueToLookFor && (col == NUM_COLS - 1))
	{
		return true;
	}

	// Recursively search for a path in adjacent cells
	if (IsValid(row - 1, col) && FindPath(valueToLookFor, row - 1, col, path, visited))
	{
		return true;
	}
	if (IsValid(row, col + 1) && FindPath(valueToLookFor, row, col + 1, path, visited))
	{
		return true;
	}
	if (IsValid(row - 1, col + 1) && FindPath(valueToLookFor, row - 1, col + 1, path, visited))
	{
		return true;
	}
	if (IsValid(row + 1, col + 1) && FindPath(valueToLookFor, row + 1, col + 1, path, visited))
	{
		return true;
	}

	path.pop_back();
	return false;
}
