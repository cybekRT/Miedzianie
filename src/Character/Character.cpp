/*
 * Character.cpp
 *
 *  Created on: 21 May 2021
 *      Author: cybek
 */

#include<map>
#include<set>
#include<list>
#include<queue>
#include<stack>
#include"main.hpp"
#include"Character.hpp"
#include"Game.hpp"
#include"Building/Building.hpp"

namespace Character
{
	Character::Character(Character::Type type, Abilities abilities, Coordinates coords, std::string sprName, int life) :
			Object(Object::Type::Character, coords.ToPosition(), CoordinatesSize(1, 1).ToSize(), sprName, life),
			abilities(abilities), type(type), movementSpeed(100), movementDeltaCounter(0),
			attackedObject(nullptr), attackSpeed(1), attackDeltaCounter(0), attackPower(2),
			gatheredObject(nullptr), gatherDelay(2000), gatherSpeed(5), gatherDeltaCounter(0)
	{

	}

	Character::~Character()
	{

	}

	void Character::Update(unsigned delta)
	{
		if(!IsAlive())
			return;

		spr.Update(delta);

		if(attackedObject || gatheredObject)
		{
			Object* target = (attackedObject) ? attackedObject : gatheredObject;

			Coordinates coords(pos);
			Coordinates targetCoords = target->GetCoordinates();
			auto targetCoordsSize = target->GetCoordinatesSize();

			int aX = targetCoords.x - 1;
			int aY = targetCoords.y - 1;
			int bX = targetCoords.x + targetCoordsSize.w + 1;
			int bY = targetCoords.y + targetCoordsSize.h + 1;

			bool inRange = (coords.x >= aX && coords.x <= bX && coords.y >= aY && coords.y <= bY);
			if(inRange)
			{
				if(attackedObject)
				{
					attackDeltaCounter += delta;

					unsigned reqAttackDelta = 1000 / attackSpeed;
					while(attackDeltaCounter >= reqAttackDelta)
					{
						attackDeltaCounter -= reqAttackDelta;
						UpdateAttack();
					}
				}
				else if(gatheredObject)
				{
					gatherDeltaCounter += delta;

					//unsigned reqGatherDelta = 1000 / gatherSpeed;
					while(gatherDeltaCounter >= gatherDelay)
					{
						gatherDeltaCounter -= gatherDelay;
						UpdateGather();
					}
				}
			}
			else
			{
				//printf("===== Attack =====");

				bool ableToMove = false;

				auto targetCSize = target->GetCoordinatesSize();
				for(unsigned y = 0; y < targetCSize.h && !ableToMove; y++)
				{
					for(unsigned x = 0; x < targetCSize.w && !ableToMove; x++)
					{
						GoTo(targetCoords + Coordinates(x, y), true);
						if(movementCoords.size() > 0)
							ableToMove = true;
					}
				}

				if(!ableToMove)
				{
					attackedObject = nullptr;
					gatheredObject = nullptr;
				}

				//printf("===== /Attack - %d =====", movementCoords.size());
			}
		}
		else
		{
			attackDeltaCounter = 0;
			gatherDeltaCounter = 0;
		}

		movementDeltaCounter += delta;
		unsigned reqDelta = 1000 / movementSpeed;
		while(movementDeltaCounter >= reqDelta)
		{
			if(movementCoords.empty())
			{
				movementDeltaCounter = 0;
				return;
			}

			movementDeltaCounter -= reqDelta;
			UpdateMovement();
		}
	}

	void Character::UpdateMovement()
	{
		//printf("Going %d/%d - %dx%d\n", reqDelta, movementDeltaCounter, pos.x, pos.y);

		if(movementCoords.empty())
			return;
		Coordinates dstCoords = movementCoords.front();
		Position dstPos = dstCoords.ToPosition();
		Position newPos = pos;

		//printf("Going from %dx%d to %dx%d\n", pos.x, pos.y, dstPos.x, dstPos.y);

		if(dstPos == pos)
		{
			movementCoords.pop();
			//printf("pop\n");

			if(movementCoords.empty())
				return;

			//movementDeltaCounter += reqDelta; // FIXME
			return;
		}
		else
		{
			if(dstPos.x > pos.x)
			{
				newPos.x++;
				//printf(">\n");
			}
			else if(dstPos.x < pos.x)
			{
				newPos.x--;
				//printf("<\n");
			}
			else if(dstPos.y > pos.y)
			{
				newPos.y++;
				//printf("^\n");
			}
			else if(dstPos.y < pos.y)
			{
				newPos.y--;
				//printf("v\n");
			}
			else
			{
				//printf("?\n");
			}
		}

		auto lvl = game->GetLevel();

		auto ncdsTopLeft = Coordinates(newPos);
		auto ncdsBotRight = Coordinates(newPos + Position(size.w - 1, size.h - 1));

		// TODO should use terrain used instead of object...
		Object* a = lvl->IsTerrainUsed(ncdsTopLeft) ? lvl->GetObject(ncdsTopLeft) : nullptr;
		Object* b = lvl->IsTerrainUsed(ncdsBotRight) ? lvl->GetObject(ncdsBotRight) : nullptr;

		if( (!a || a == this) && (!b || b == this) )
		{
			//printf("Pos: %dx%d -> %dx%d\n", pos.x, pos.y, newPos.x, newPos.y);
			// Old position
			auto curTopLeft = Coordinates(pos);
			auto curBotRight = Coordinates(pos + Position(size.w - 1, size.h - 1));
			lvl->SetObject(curTopLeft, nullptr);
			lvl->SetObject(curBotRight, nullptr);
			//printf("Current: %dx%d, %dx%d\n", curTopLeft.x, curTopLeft.y, curBotRight.x, curBotRight.y);
			// New position
			lvl->SetObject(ncdsTopLeft, this);
			lvl->SetObject(ncdsBotRight, this);
			//printf("    New: %dx%d, %dx%d\n", ncdsTopLeft.x, ncdsTopLeft.y, ncdsBotRight.x, ncdsBotRight.y);

			pos = newPos;
		}
		else
		{
			//printf("... can't ...\n");
			/*Coordinates dstCoords;
			while(!movementCoords.empty())
			{
				dstCoords = movementCoords.front();
				movementCoords.pop();
			}

			GoTo(dstCoords);*/
		}
	}

	void Character::UpdateAttack()
	{
		if(!attackedObject)
			return;

		attackedObject->DealDamage(attackPower);
		PlaySound(soundAttack);

		if(attackedObject->GetLife() <= 0)
			attackedObject = nullptr;
	}

	void Character::UpdateGather()
	{
		if(!gatheredObject)
			return;

		game->AddCopper(gatherSpeed);
		game->GetLevel()->AddOneShotSprite("spark", gatheredObject->GetCoordinates());
	}

	void Character::GoTo(Coordinates dstCoords, bool attackingOrGathering)
	{
		if(!attackingOrGathering)
		{
			attackedObject = nullptr;
			gatheredObject = nullptr;
		}

		//printf("%s go to: %dx%d\n", sprDefName.c_str(), dstCoords.x, dstCoords.y);
		Coordinates curCoords = pos;

		auto lvl = game->GetLevel();

		// Clear prev movement...
		if(!movementCoords.empty())
		{
			Coordinates lastCoords = movementCoords.front();

			while(!movementCoords.empty())
				movementCoords.pop();

			if(Coordinates(pos).ToPosition() != pos)
			{
				movementCoords.push(lastCoords);
				curCoords = lastCoords;
			}
		}

		//if(!lvl->IsCoordinatesValid(dstCoords) || lvl->GetObject(dstCoords)) // FIXME - attack enemies?
		//	return;
		if(lvl->IsTerrainUsed(dstCoords) && !lvl->GetObject(dstCoords))
			return;

		struct MoveInfo
		{
			Coordinates from;
			int len;

			MoveInfo() : from(Coordinates(-1, -1)), len(-1) {}
			MoveInfo(Coordinates from, int len) : from(from), len(len) {}
		};

		std::map<Coordinates, MoveInfo> coordsInfo;
		std::queue<Coordinates> toVisit;

		toVisit.push(curCoords);
		coordsInfo[curCoords] = MoveInfo(curCoords, 0);

		//printf("===== Searching %dx%d -> %dx%d =====\n", curCoords.x, curCoords.y, dstCoords.x, dstCoords.y);
		while(!toVisit.empty())
		{
			auto curCoords = toVisit.front();
			toVisit.pop();

			if(!lvl->IsCoordinatesValid(curCoords))
				continue;

			//Object* objAt = lvl->GetObject(curCoords);
			Object* objAt = lvl->IsTerrainUsed(curCoords) ? lvl->GetObject(curCoords) : nullptr;
			//if(objAt && objAt != this)// && lvl->IsTerrainUsed(curCoords))
			if(lvl->IsTerrainUsed(curCoords) && (!objAt || objAt != this))
				continue;

			//printf("-> %dx%d\n", curCoords.x, curCoords.y);

			if(curCoords == dstCoords)
			{
				//printf("Break~!\n");
				break;
			}

			auto& info = coordsInfo[curCoords];
			//if(info.len > 0)
			//	continue;

			//info.len = coordsInfo[info.from].len + 1;

			auto checkCoords = [curCoords, &coordsInfo, info, &toVisit](Coordinates coords)
			{
				auto& curInfo = coordsInfo[coords];
				if(curInfo.len < 0 || info.len + 1 < curInfo.len)
				{
					curInfo.from = curCoords;
					curInfo.len = info.len + 1;
					toVisit.push(coords);
				}
			};

			checkCoords(curCoords + Coordinates(-1,  0));
			checkCoords(curCoords + Coordinates( 1,  0));
			checkCoords(curCoords + Coordinates( 0, -1));
			checkCoords(curCoords + Coordinates( 0,  1));
		}

		auto dstInfo = coordsInfo[dstCoords];
		//printf("Dst info: %d - %dx%d\n", dstInfo.len, dstInfo.from.x, dstInfo.from.y);

		if(dstInfo.len > 0 && lvl->IsCoordinatesValid(dstInfo.from)) // :'(
		{
			std::stack<Coordinates> revPath;
			//revPath.push(dstCoords); // -_____________-""
			//printf("=== Path ===\n");
			//while(dstInfo.from != curCoords)
			do
			{
				Object* objAt = lvl->GetObject(dstCoords);
				//printf("?+ %d %d\n", !objAt, (objAt) ? objAt->GetObjectType() != Object::Type::Building : -1);
				if(!objAt || objAt->GetObjectType() != Object::Type::Building || !objAt->IsAlive())
				{
					revPath.push(dstCoords);
				}

				//printf("- %dx%d\n", dstInfo.from.x, dstInfo.from.y);

				dstCoords = dstInfo.from;
				dstInfo = coordsInfo[dstInfo.from];
			}
			while(dstCoords != curCoords);

			while(!revPath.empty())
			{
				auto coords = revPath.top();
				revPath.pop();
				movementCoords.push(coords);
			}
		}

		//printf("Steps: %d\n", movementCoords.size());
		//if(movementCoords.size())
		//	printf("> %dx%d\n", movementCoords.front().x, movementCoords.front().y);
	}

	void Character::Attack(Object* obj)
	{
		if(!abilities.canAttack || obj == this)
			return;

		attackedObject = obj;
		gatheredObject = nullptr;
	}

	void Character::Gather(Object* obj)
	{
		if(!abilities.canGather)
			return;

		if(obj->GetObjectType() != Object::Type::Building)
			return;

		auto building = (Building::Building*)obj;
		if(building->GetBuildingType() != Building::Building::Type::Mine)
			return;

		attackedObject = nullptr;
		gatheredObject = obj;
	}
}
