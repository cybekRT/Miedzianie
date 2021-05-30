#pragma once
/*
 * Track.hpp
 *
 *  Created on: 18 May 2021
 *      Author: cybek
 */

#include"Building.hpp"

namespace Building
{
	class Track : public Building
	{
		protected:
			enum Direction
			{
				Left 	= 1, // 0001
				Right 	= 2, // 0010
				Up 		= 4, // 0100
				Down 	= 8, // 1000
			};

		public:
			Track(Coordinates coords);
			virtual ~Track(); // -_-"

			void Update(unsigned delta);
			void UpdateOnLevelChange();
	};
}
