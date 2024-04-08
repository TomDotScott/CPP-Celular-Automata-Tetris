#pragma once
#include <SFML/Config.hpp>
#include <SFML/System/Vector2.hpp>

class Tetromino
{
public:
	class Rotation
	{
	public:
		Rotation(sf::Uint32* shape, int width, int height);
		~Rotation();

		int GetWidth() const { return m_width; }
		int GetHeight() const { return m_height; }
		const sf::Uint32* GetBuffer() const { return m_buffer; }

	private:
		sf::Uint32* m_buffer;
		const int m_width;
		const int m_height;
	};

	enum class eRotation
	{
		// TODO: Do they need values? Might be useful
		e_up = 0,
		e_left = 90,
		e_down = 180,
		e_right = 270
	};

	Tetromino();

	const Rotation* GetShape() const { return m_currentShape; }

	const sf::Vector2i& GetPosition() const { return m_position; }

	void MoveLeft(int amount);
	void MoveRight(int amount);
	void MoveDown(int amount);

	virtual void Rotate();

protected:
	Rotation* m_currentShape = nullptr;

	eRotation m_currentRotation;

private:
	sf::Vector2i m_position;
};

class LBlock final : public Tetromino
{
public:
	LBlock();
	void Rotate() override;

private:
	Rotation m_upRotation;
	Rotation m_leftRotation;
	Rotation m_downRotation;
	Rotation m_rightRotation;
};

class JBlock final : public Tetromino
{
public:
	JBlock();
	void Rotate() override;

private:
	Rotation m_upRotation;
	Rotation m_leftRotation;
	Rotation m_downRotation;
	Rotation m_rightRotation;
};

class IBlock final : public Tetromino
{
public:
	IBlock();
	void Rotate() override;

private:
	Rotation m_upRotation;
	Rotation m_leftRotation;
};

class OBlock final : public Tetromino
{
public:
	OBlock();

private:
	Rotation m_upRotation;
};

class SBlock final : public Tetromino
{
public:
	SBlock();
	void Rotate() override;

private:
	Rotation m_upRotation;
	Rotation m_leftRotation;
};

class TBlock final : public Tetromino
{
public:
	TBlock();
	void Rotate() override;

private:
	Rotation m_upRotation;
	Rotation m_leftRotation;
	Rotation m_downRotation;
	Rotation m_rightRotation;
};

class ZBlock final : public Tetromino
{
public:
	ZBlock();
	void Rotate() override;

private:
	Rotation m_upRotation;
	Rotation m_leftRotation;
};
