/*
 * Power.cpp
 *
 *  Created on: 19 May 2021
 *      Author: cybek
 */

#include"Power.hpp"

namespace Building
{
	Power::Power(Coordinates coords) :
			Building(Building::Type::Power, coords, CoordinatesSize(1, 2), "power2", 5)
	{

	}

	Power::~Power()
	{

	}

	void Power::UpdateOnLevelChange()
	{
		if(grounded)
		{
			//life--;
			DealDamage(1);
		}
	}
}
