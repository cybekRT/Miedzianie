/*
 * Building.cpp
 *
 *  Created on: 19 May 2021
 *      Author: cybek
 */

#include"Building.hpp"
#include<set>
#include<queue>
#include"Game.hpp"

namespace Building
{
	Building::Building(Building::Type type, Coordinates coords, CoordinatesSize coordsSize, std::string sprName, int life) :
			Object(Object::Type::Building, coords.ToPosition(), coordsSize.ToSize(), sprName, life),
			type(type), grounded(false), powered(false)
	{
//		Level* lvl = game->GetLevel();
//
//		lvl->AddObject(this);
//		lvl->SetObject(coords, this);
		soundDie = "building_destroyed.ogg";
	}

	Building::~Building()
	{
		printf("### WTF~! ###\n"); for(;;);
	}

	void Building::Draw(SDL_Surface* buffer, Position camera)
	{
		Object::Draw(buffer, camera);

		if(grounded)
			Sprite("grounded", pos).Draw(buffer, camera);
		if(powered)
			Sprite("powered", pos).Draw(buffer, camera);
	}
}
