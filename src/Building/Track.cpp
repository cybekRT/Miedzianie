/*
 * Track.cpp
 *
 *  Created on: 19 May 2021
 *      Author: cybek
 */

#include "Track.hpp"

#include"Game.hpp"

namespace Building
{
	Track::Track(Coordinates coords) :
			Building(Building::Type::Track, coords, CoordinatesSize(1, 1), "track2", 4)
	{

	}

	Track::~Track()
	{

	}

	void Track::Update(unsigned delta)
	{
		if(life <= 0)
			return;

		if(grounded && powered)
		{
			SetLife(0, true);

			auto frame = spr.GetFrame();
			spr = Sprite("track2_dead", pos);
			spr.SetFrame(frame);
		}
	}

	void Track::UpdateOnLevelChange()
	{
		if(life <= 0)
			return;

		Coordinates coords = Coordinates(pos);

		int type = 0;

		Level* lvl = game->GetLevel();

		auto isTrackAt /* or other building */ = [this, lvl](Coordinates c) -> bool {
			Object* obj = lvl->GetObject(c);
			if(!obj || !obj->IsAlive() || obj->GetObjectType() != Object::Type::Building)
				return false;

			Building* b = (Building*)obj;
			return b->GetBuildingType() != Building::Type::Ground
					&& b->GetBuildingType() != Building::Type::Power;
		};

		auto isGroundOrPowerAt = [this, lvl](Coordinates c) -> bool {
			Object* obj = lvl->GetObject(c);
			if(!obj || !obj->IsAlive() || obj->GetObjectType() != Object::Type::Building)
				return false;

			Building* b = (Building*)obj;
			return b->GetBuildingType() == Building::Type::Power ||
					b->GetBuildingType() == Building::Type::Ground;
		};

		if(isTrackAt(coords + Coordinates(-1, 0)))
			type |= Direction::Left;
		if(isTrackAt(coords + Coordinates( 1, 0)))
			type |= Direction::Right;

		if(isTrackAt(coords + Coordinates(0, -1)))
			type |= Direction::Up;
		if(isTrackAt(coords + Coordinates(0,  1)) || isGroundOrPowerAt(coords + Coordinates(0, 1)))
			type |= Direction::Down;

		spr.SetFrame(type);
	}
}
