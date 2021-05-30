/*
 * MCU.cpp
 *
 *  Created on: 20 May 2021
 *      Author: cybek
 */

#include"MCU.hpp"

namespace Building
{
	MCU::MCU(Coordinates coords) : Building(Building::Type::Other, coords, CoordinatesSize(2, 2), "mcu", 100)
	{

	}

	MCU::~MCU()
	{

	}
}
