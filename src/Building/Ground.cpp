/*
 * Ground.cpp
 *
 *  Created on: 19 May 2021
 *      Author: cybek
 */

#include"Ground.hpp"

namespace Building
{
	Ground::Ground(Coordinates coords) :
			Building(Building::Type::Ground, coords, CoordinatesSize(1, 1), "ground", 5)
	{

	}

	Ground::~Ground()
	{

	}

	void Ground::UpdateOnLevelChange()
	{
		if(powered)
		{
			//life--;
			DealDamage(1);
		}
	}
}
