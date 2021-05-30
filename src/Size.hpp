#pragma once
/*
 * Size.hpp
 *
 *  Created on: 18 May 2021
 *      Author: cybek
 */

struct Size
{
	unsigned w, h;

	Size() : w(0), h(0) {}
	Size(unsigned w, unsigned h) : w(w), h(h) {}
};

struct CoordinatesSize
{
	unsigned w, h;

	CoordinatesSize() : w(0), h(0) {}
	CoordinatesSize(unsigned w, unsigned h) : w(w), h(h) {}

	CoordinatesSize(Size size) : w(size.w / 32), h(size.h / 32) {}
	Size ToSize() { return Size(w * 32, h * 32); }
};
