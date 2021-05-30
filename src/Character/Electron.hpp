#pragma once
/*
 * Electron.hpp
 *
 *  Created on: 21 May 2021
 *      Author: cybek
 */

#include"Character.hpp"

namespace Character
{
	class Electron : public Character
	{
		public:
			Electron(Coordinates coords);
			virtual ~Electron();
	};
}
