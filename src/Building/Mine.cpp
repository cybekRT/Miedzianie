/*
 * Mine.cpp
 *
 *  Created on: 22 May 2021
 *      Author: cybek
 */

#include"Mine.hpp"

namespace Building
{
	Mine::Mine(Coordinates coords) : Building(Building::Type::Mine, coords, CoordinatesSize(2, 2), "mine", 1000)
	{

	}

	Mine::~Mine()
	{

	}
}
