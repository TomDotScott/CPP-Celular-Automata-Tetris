#pragma once
#include <SFML/Config.hpp>
#include <SFML/System/Vector2.hpp>

class Tetromino
{
public:
	static constexpr unsigned TETROMINO_PIXEL_WIDTH = 4;
	static constexpr unsigned TETROMINO_PIXEL_HEIGHT = 2;

	#define SPRITE sf::Uint32 m_shape[TETROMINO_PIXEL_WIDTH * TETROMINO_PIXEL_HEIGHT]

	const sf::Uint32* GetBuffer() const { return m_buffer; }

	const sf::Vector2i& GetPosition() const { return m_position; }

	void MoveLeft(const int amount);
	void MoveRight(int amount);
	void MoveDown(int amount);

protected:
	sf::Uint32* m_buffer = nullptr;

private:
	sf::Vector2i m_position;
};

class LBlock final : public Tetromino
{
public:
	LBlock()
	{
		m_buffer = m_shape;
	}

private:
	SPRITE
	{
		0x00000000, 0x00000000, 0x00000000, 0xFFA500FF,
		0xFFA500FF, 0xFFA500FF, 0xFFA500FF, 0xFFA500FF
	};
};

class JBlock final : public Tetromino
{
public:
	JBlock()
	{
		m_buffer = m_shape;
	}

private:
	SPRITE
	{
		0xADD8E6FF, 0x00000000, 0x00000000, 0x00000000,
		0xADD8E6FF, 0xADD8E6FF, 0xADD8E6FF, 0xADD8E6FF
	};
};

class IBlock final : public Tetromino
{
public:
	IBlock()
	{
		m_buffer = m_shape;
	}

private:
	SPRITE
	{
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00008BFF, 0x00008BFF, 0x00008BFF, 0x00008BFF
	};
};

class OBlock final : public Tetromino
{
public:
	OBlock()
	{
		m_buffer = m_shape;
	}

private:
	SPRITE
	{
		0xFFFF00FF, 0xFFFF00FF, 0x00000000, 0x00000000,
		0xFFFF00FF, 0xFFFF00FF, 0x00000000, 0x00000000
	};
};

class SBlock final : public Tetromino
{
public:
	SBlock()
	{
		m_buffer = m_shape;
	}

private:
	SPRITE
	{
		0x00000000, 0x90EE90FF, 0x90EE90FF, 0x00000000,
		0x90EE90FF, 0x90EE90FF, 0x00000000, 0x00000000
	};
};

class TBlock final : public Tetromino
{
public:
	TBlock()
	{
		m_buffer = m_shape;
	}

private:
	SPRITE
	{
		0x00000000, 0xFF00FFFF, 0x00000000, 0x00000000,
		0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0x00000000
	};
};

class ZBlock final : public Tetromino
{
public:
	ZBlock()
	{
		m_buffer = m_shape;
	}

private:
	SPRITE
	{
		0xBF0000FF, 0xBF0000FF, 0x00000000, 0x00000000,
		0x00000000, 0xBF0000FF, 0xBF0000FF, 0x00000000
	};
};
