#pragma once
/*
 * Oxygen.hpp
 *
 *  Created on: 23 May 2021
 *      Author: cybek
 */

#include"Character.hpp"

namespace Character
{
	class Oxygen : public Character
	{
		protected:
			int scanRange;

		public:
			Oxygen(Coordinates coords);
			virtual ~Oxygen();

			void Update(unsigned delta) override;
	};
}
