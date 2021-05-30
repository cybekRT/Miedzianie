#pragma once
/*
 * Power.hpp
 *
 *  Created on: 19 May 2021
 *      Author: cybek
 */

#include"Building.hpp"

namespace Building
{
	class Power : public Building
	{
		public:
			Power(Coordinates coords);
			~Power();

			void UpdateOnLevelChange();
	};
}
