#include "Tetromino.h"

#include "Game.h"

Tetromino::Rotation::Rotation(sf::Uint32* shape, const int width, const int height) :
	m_buffer(nullptr),
	m_width(width),
	m_height(height)
{
	const size_t bytes = static_cast<size_t>(m_width) * static_cast<size_t>(m_height) * sizeof(sf::Uint32);

	m_buffer = static_cast<sf::Uint32*>(malloc(bytes));

	memcpy(m_buffer, shape, bytes);

	delete[] shape;
}

Tetromino::Rotation::~Rotation()
{
	delete[] m_buffer;
}

Tetromino::Tetromino() :
	m_currentRotation(eRotation::e_up),
	m_position(SCREEN_WIDTH / 2, 0)
{
}

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

void Tetromino::Rotate()
{
	switch (m_currentRotation)
	{
	case eRotation::e_up:
		m_currentRotation = eRotation::e_left;
		break;
	case eRotation::e_left:
		m_currentRotation = eRotation::e_down;
		break;
	case eRotation::e_down:
		m_currentRotation = eRotation::e_right;
		break;
	case eRotation::e_right:
		m_currentRotation = eRotation::e_up;
		break;
	}
}

LBlock::LBlock() :
	m_upRotation(new sf::Uint32[6]
		{
			0xFFA500FF, 0x00000000,
			0xFFA500FF, 0x00000000,
			0xFFA500FF, 0xFFA500FF
		}, 2, 3),
	m_leftRotation(new sf::Uint32[6]
		{
			0x00000000, 0x00000000, 0xFFA500FF,
			0xFFA500FF, 0xFFA500FF, 0xFFA500FF
		}, 3, 2),
	m_downRotation(new sf::Uint32[6]
		{
			0xFFA500FF, 0xFFA500FF,
			0x00000000, 0xFFA500FF,
			0x00000000, 0xFFA500FF
		}, 2, 3),
	m_rightRotation(new sf::Uint32[6]
		{
			0xFFA500FF, 0xFFA500FF, 0xFFA500FF,
			0xFFA500FF, 0x00000000, 0x00000000
		}, 3, 2)
{
	m_currentShape = &m_upRotation;
}

void LBlock::Rotate()
{
	Tetromino::Rotate();

	switch (m_currentRotation)
	{
	case eRotation::e_up:
		m_currentShape = &m_upRotation;
		break;
	case eRotation::e_right:
		m_currentShape = &m_rightRotation;
		break;
	case eRotation::e_down:
		m_currentShape = &m_downRotation;
		break;
	case eRotation::e_left:
		m_currentShape = &m_leftRotation;
		break;
	}
}

JBlock::JBlock() :
	m_upRotation(new sf::Uint32[6]
		{
			0x00000000, 0xADD8E6FF,
			0x00000000, 0xADD8E6FF,
			0xADD8E6FF, 0xADD8E6FF
		}, 2, 3),
	m_leftRotation(new sf::Uint32[6]
		{
			0xADD8E6FF, 0x00000000, 0x00000000,
			0xADD8E6FF, 0xADD8E6FF, 0xADD8E6FF
		}, 3, 2),
	m_downRotation(new sf::Uint32[6]
		{
			0xADD8E6FF, 0xADD8E6FF,
			0xADD8E6FF, 0x00000000,
			0xADD8E6FF, 0x00000000
		}, 2, 3),
	m_rightRotation(new sf::Uint32[6]
		{
			0xADD8E6FF, 0xADD8E6FF, 0xADD8E6FF,
			0x00000000, 0x00000000, 0xADD8E6FF
		}, 3, 2)
{
	m_currentShape = &m_upRotation;
}

void JBlock::Rotate()
{
	Tetromino::Rotate();

	switch (m_currentRotation)
	{
	case eRotation::e_up:
		m_currentShape = &m_upRotation;
		break;
	case eRotation::e_right:
		m_currentShape = &m_rightRotation;
		break;
	case eRotation::e_down:
		m_currentShape = &m_downRotation;
		break;
	case eRotation::e_left:
		m_currentShape = &m_leftRotation;
		break;
	}
}

IBlock::IBlock() :
	m_upRotation(new sf::Uint32[4]
		{
			0x00008BFF,
			0x00008BFF,
			0x00008BFF,
			0x00008BFF,
		}, 1, 4),
		m_leftRotation(new sf::Uint32[4]
			{
				0x00008BFF, 0x00008BFF, 0x00008BFF, 0x00008BFF
			}, 4, 1)
{
	m_currentShape = &m_upRotation;
}

void IBlock::Rotate()
{
	Tetromino::Rotate();

	switch (m_currentRotation)
	{
	case eRotation::e_up:
	case eRotation::e_down:
		m_currentShape = &m_upRotation;
		break;
	case eRotation::e_left:
	case eRotation::e_right:
		m_currentShape = &m_leftRotation;
		break;
	}
}

OBlock::OBlock() :
	m_upRotation({
		new sf::Uint32[4]{
			0xFFFF00FF, 0xFFFF00FF,
			0xFFFF00FF, 0xFFFF00FF
		}, 2, 2
		})
{
	m_currentShape = &m_upRotation;
}

SBlock::SBlock() :
	m_upRotation({
		new sf::Uint32[6]{
			0x00000000, 0x90EE90FF, 0x90EE90FF,
			0x90EE90FF, 0x90EE90FF, 0x00000000
		}, 3, 2
		}),
	m_leftRotation({
		new sf::Uint32[6]{
			0x90EE90FF, 0x00000000,
			0x90EE90FF, 0x90EE90FF,
			0x00000000, 0x90EE90FF
		}, 2, 3
		})
{
	m_currentShape = &m_upRotation;
}

void SBlock::Rotate()
{
	Tetromino::Rotate();

	switch (m_currentRotation) {
	case eRotation::e_up:
	case eRotation::e_down:
		m_currentShape = &m_upRotation;
		break;
	case eRotation::e_left:
	case eRotation::e_right:
		m_currentShape = &m_leftRotation;
		break;
	}
}

TBlock::TBlock() :
	m_upRotation(new sf::Uint32[6]
		{
			0x00000000, 0xFF00FFFF, 0x00000000,
			0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF
		}, 3, 2),
	m_leftRotation(new sf::Uint32[6]
		{
			0xFF00FFFF, 0x00000000,
			0xFF00FFFF, 0xFF00FFFF,
			0xFF00FFFF, 0x00000000
		}, 2, 3),
	m_downRotation(new sf::Uint32[6]
		{
			0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF,
			0x00000000, 0xFF00FFFF, 0x00000000
		}, 3, 2),
	m_rightRotation(new sf::Uint32[6]
		{
			0x00000000, 0xFF00FFFF,
			0xFF00FFFF, 0xFF00FFFF,
			0x00000000, 0xFF00FFFF
		}, 2, 3)
{
	m_currentShape = &m_upRotation;
}

void TBlock::Rotate()
{
	Tetromino::Rotate();

	switch (m_currentRotation)
	{
	case eRotation::e_up:
		m_currentShape = &m_upRotation;
		break;
	case eRotation::e_right:
		m_currentShape = &m_rightRotation;
		break;
	case eRotation::e_down:
		m_currentShape = &m_downRotation;
		break;
	case eRotation::e_left:
		m_currentShape = &m_leftRotation;
		break;
	}
}

ZBlock::ZBlock() :
	m_upRotation({
		new sf::Uint32[6]{
			0xBF0000FF, 0xBF0000FF, 0x00000000,
			0x00000000, 0xBF0000FF, 0xBF0000FF
		}, 3, 2
		}),
	m_leftRotation({
		new sf::Uint32[6]{
			0x00000000, 0xBF0000FF,
			0xBF0000FF, 0xBF0000FF,
			0xBF0000FF, 0x00000000
		}, 2, 3
		})
{
	m_currentShape = &m_upRotation;
}

void ZBlock::Rotate()
{
	Tetromino::Rotate();

	switch (m_currentRotation) {
	case eRotation::e_up:
	case eRotation::e_down:
		m_currentShape = &m_upRotation;
		break;
	case eRotation::e_left:
	case eRotation::e_right:
		m_currentShape = &m_leftRotation;
		break;
	}
}
