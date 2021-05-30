#pragma once
/*
 * Position.hpp
 *
 *  Created on: 17 May 2021
 *      Author: cybek
 */

struct Position
{
	int x, y;

	Position() : x(0), y(0) {}
	Position(int x, int y) : x(x), y(y) {}

	Position operator+(const Position& arg)
	{
		return Position(x + arg.x, y + arg.y);
	}

	Position operator-(const Position& arg)
	{
		return Position(x - arg.x, y - arg.y);
	}

	bool operator!=(const Position& arg) const
	{
		return x != arg.x || y != arg.y;
	}

	bool operator==(const Position& arg) const
	{
		return x == arg.x && y == arg.y;
	}
};

struct Coordinates
{
	int x, y;

	Coordinates() : x(0), y(0) {}
	Coordinates(int x, int y) : x(x), y(y) {}

	Coordinates(Position pos) : x(pos.x / 32), y(pos.y / 32) {}
	Position ToPosition() { return Position(x * 32, y * 32); }

	Coordinates operator+(const Coordinates& arg)
	{
		return Coordinates(x + arg.x, y + arg.y);
	}

	Coordinates operator-(const Coordinates& arg)
	{
		return Coordinates(x - arg.x, y - arg.y);
	}

	bool operator<(const Coordinates& arg) const
	{
		int a = y * 99999 + x;
		int b = arg.y * 99999 + arg.x;

		return a < b;
	}

	bool operator!=(const Coordinates& arg) const
	{
		return x != arg.x || y != arg.y;
	}

	bool operator==(const Coordinates& arg) const
	{
		return x == arg.x && y == arg.y;
	}
};
