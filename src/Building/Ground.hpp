#pragma once
/*
 * Ground.hpp
 *
 *  Created on: 19 May 2021
 *      Author: cybek
 */

#include"Building.hpp"

namespace Building
{
	class Ground : public Building
	{
		public:
			Ground(Coordinates coords);
			~Ground();

			void UpdateOnLevelChange();
	};
}
