#pragma once
/*
 * Mine.hpp
 *
 *  Created on: 22 May 2021
 *      Author: cybek
 */

#include"Building.hpp"

namespace Building
{
	class Mine : public Building
	{
		public:
			Mine(Coordinates coords);
			~Mine();
	};
}


