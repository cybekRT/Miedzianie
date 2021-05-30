/*
 * Oxygen.cpp
 *
 *  Created on: 23 May 2021
 *      Author: cybek
 */

#include"main.hpp"
#include"Oxygen.hpp"
#include"Game.hpp"

namespace Character
{
	Oxygen::Oxygen(Coordinates coords) :
			Character(Character::Type::Enemy,
			Character::Abilities { .canAttack = true, .canGather = false },
			coords, "oxygen", 2),
			scanRange(5)
	{
		soundAttack = "oxygen_attack.ogg";
	}

	Oxygen::~Oxygen()
	{

	}

	void Oxygen::Update(unsigned delta)
	{
		if(!IsAlive())
			return;

		if(!attackedObject || attackedObject->GetObjectType() != Object::Type::Character)
		{
			bool finish = false;
			auto lvl = game->GetLevel();
			for(int currentRange = 1; currentRange <= scanRange && !finish; currentRange++)
			{
				for(int y = -currentRange; y < currentRange && !finish; y++)
				{
					for(int x = -currentRange; x < currentRange && !finish; x++)
					{
						auto checkCoords = Coordinates(pos) + Coordinates(x, y);
						auto obj = lvl->IsTerrainUsed(checkCoords) ? lvl->GetObject(checkCoords) : nullptr;
						if(!obj)
							continue;

						if(obj->GetObjectType() == Object::Type::Building)
						{
							if(!attackedObject)
								Attack(obj);
						}
						else
						{
							auto c = (Character*)obj;
							if(c->GetCharacterType() == Character::Type::Player)
							{
								Attack(c);
								finish = true;
							}
						}
					}
				}
			}
		}

		Character::Update(delta);
	}
}
