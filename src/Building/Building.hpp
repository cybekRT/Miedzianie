#pragma once
/*
 * Building.hpp
 *
 *  Created on: 18 May 2021
 *      Author: cybek
 */

#include"Object.hpp"

namespace Building
{
	class Building : public Object
	{
		public:
			enum class Type
			{
				Track,
				Ground,
				Power,
				Mine,

				Other // ????
			};

		protected:
			Type type;
			bool grounded;
			bool powered;

		public:
			Building(Building::Type type, Coordinates coords, CoordinatesSize coordsSize, std::string sprName, int life);
			virtual ~Building();

			virtual void Draw(SDL_Surface* buffer, Position camera);

			Type GetBuildingType() { return type; }

			bool IsPowered() { return powered; }
			void SetPowered(bool powered) { this->powered = powered; }

			bool IsGrounded() { return grounded; }
			void SetGrounded(bool grounded) { this->grounded = grounded; }
	};
}
