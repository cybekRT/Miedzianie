#pragma once
/*
 * MCU.hpp
 *
 *  Created on: 20 May 2021
 *      Author: cybek
 */

#include"Building.hpp"

namespace Building
{
	class MCU : public Building
	{
		public:
			MCU(Coordinates coords);
			~MCU();
	};
}
