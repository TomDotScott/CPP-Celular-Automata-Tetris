#include "Tetromino.h"

void Tetromino::MoveLeft(const int amount)
{
	m_position.x -= amount;
}

void Tetromino::MoveRight(const int amount)
{
	m_position.x += amount;
}

void Tetromino::MoveDown(const int amount)
{
	m_position.y += amount;
}
