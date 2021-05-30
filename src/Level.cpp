/*
 * Level.cpp
 *
 *  Created on: 17 May 2021
 *      Author: cybek
 */

#include<fstream>
#include<set>
#include<list>
#include<queue>
#include"main.hpp"
#include"Level.hpp"
#include"Building/Building.hpp"

/* Building */
#include "Building/Track.hpp"
#include "Building/Power.hpp"
#include "Building/Ground.hpp"
#include "Building/MCU.hpp"
#include "Building/Mine.hpp"
/* Characters */
#include "Character/Electron.hpp"
#include "Character/Oxygen.hpp"

Level::Level(std::string path) : terrain(nullptr), w(0), h(0), wasChanged(true),
		enemyUnits(0), playerUnits(0)
{
	std::ifstream f("data/" + path);
	if(!f)
		throw "Oopsie...";

	f >> w >> h;
	terrain = new unsigned[w * h];
	terrainUsed = new bool[w * h];
	terrainObjects = new Object*[w * h];

	struct ObjInfo
	{
		std::string type;
		Coordinates coords;
	};

	std::queue<ObjInfo> objsToCreate;

	std::string tmp;
	for(unsigned y = 0; y < h; y++)
	{
		for(unsigned x = 0; x < w; x++)
		{
			unsigned type = 0;
			f >> tmp;
			try
			{
				type = std::stoi(tmp);
				//printf("%s == %d\n", tmp.c_str(), type);
			}
			catch(std::invalid_argument&)
			{
				objsToCreate.push(ObjInfo { .type = tmp, .coords = Coordinates(x, y) } );
				//printf("Object at: %dx%d\n", x, y);
				type = 0;
			}

			terrain[y * w + x] 		= type;
			terrainUsed[y * w + x] 	= (type != 0);

			terrainObjects[y * w + x] = nullptr;
		}
	}

	if(f.fail())
		throw "Oopsie2...";

	f.close();

	while(!objsToCreate.empty())
	{
		auto objInfo = objsToCreate.front();
		objsToCreate.pop();

		printf("Creating obj: \"%s\"\n", objInfo.type.c_str());

		Object* obj = nullptr;
		//#include "Building/Track.hpp"
		if(objInfo.type == "t")
			obj = new Building::Track(objInfo.coords);
		//#include "Building/Power.hpp"
		if(objInfo.type == "p")
			obj = new Building::Power(objInfo.coords);
		//#include "Building/Ground.hpp"
		if(objInfo.type == "g")
			obj = new Building::Ground(objInfo.coords);
		//#include "Building/MCU.hpp"
		if(objInfo.type == "mc")
			obj = new Building::MCU(objInfo.coords);
		//#include "Building/Mine.hpp"
		if(objInfo.type == "mi")
			obj = new Building::Mine(objInfo.coords);
		//#include "Character/Electron.hpp"
		if(objInfo.type == "e")
			obj = new Character::Electron(objInfo.coords);
		//#include "Character/Oxygen.hpp"
		if(objInfo.type == "o")
			obj = new Character::Oxygen(objInfo.coords);

		if(obj)
		{
			printf("Adding obj~!\n");
			AddObject(obj);
		}
	}

	tiles = LoadIMG("tiles.png");
	if(!tiles)
		throw "Oopsie3...";

	// XXX
	usedSrf = LoadIMG("used.png");
	freeSrf = LoadIMG("free.png");
}

Level::~Level()
{
	SDL_FreeSurface(tiles);
	delete[] terrain;
}

void Level::Update(unsigned delta)
{
	for(auto obj : objects)
	{
		obj->Update(delta);
	}

	for(auto& spr : oneShotSprites)
	{
		spr.Update(delta);
	}

	if(wasChanged)
	{
		UpdateConnections();

		for(auto obj : objects)
			obj->UpdateOnLevelChange();

		wasChanged = false;
	}

	// Remove diedededed objects
	enemyUnits = 0;
	playerUnits = 0;
	for(auto itr = objects.begin(); itr != objects.end(); )
	{
		if((*itr)->GetLife() < 0)
			objects.erase(itr++);
		else
		{
			if((*itr)->GetLife() > 0)
			{
				if((*itr)->GetObjectType() == Object::Type::Character)
				{
					auto c = (Character::Character*)(*itr);
					if(c->GetCharacterType() == Character::Character::Type::Player)
						playerUnits++;
					else if(c->GetCharacterType() == Character::Character::Type::Enemy)
						enemyUnits++;
				}
			}

			itr++;
		}
	}
}

void Level::Update(SDL_Event& ev)
{

}

void Level::Draw(SDL_Surface* buffer, Position camera)
{
	DrawTerrain(buffer, camera);
	DrawObjects(buffer, camera);
}

void Level::DrawTerrain(SDL_Surface* buffer, Position camera)
{
	for(unsigned y = 0; y < h; y++)
	{
		for(unsigned x = 0; x < w; x++)
		{
			int type = terrain[y * w + x];
			int sx = (type * 32) % tiles->w;
			int sy = (type * 32) / tiles->w * 32;

			SDL_Rect src, dst;
			src.x = sx;
			src.y = sy;
			src.w = src.h = 32;

			dst.x = x * 32 - camera.x;
			dst.y = y * 32 - camera.y;
			dst.w = dst.h = 32;

			SDL_BlitSurface(tiles, &src, buffer, &dst);

			/*bool used = terrainUsed[y * w + x];
			SDL_BlitSurface((used) ? usedSrf : freeSrf, nullptr, buffer, &dst);*/

			/*Object* obj = terrainObjects[y * w + x];
			if(obj)
				obj->Draw(buffer, camera);*/
		}
	}
}

void Level::DrawObjects(SDL_Surface* buffer, Position camera)
{
	for(auto obj : objects)
		obj->Draw(buffer, camera);

	for(auto& spr : oneShotSprites)
		spr.Draw(buffer, camera);
}

void Level::AddObject(Object* obj)
{
	objects.push_back(obj);

	CoordinatesSize csize = obj->GetCoordinatesSize();
	for(unsigned y = 0; y < csize.h; y++) // -_-"
	{
		for(unsigned x = 0; x < csize.w; x++)
		{
			Coordinates coords = Coordinates(obj->GetPosition()) + Coordinates(x, y);
			SetObject(coords, obj);
		}
	}

	wasChanged = true;
}

void Level::RemoveObject(Object* obj)
{
	obj->SetLife(-1);
	objects.remove(obj);
	// Check terrain?
}

Object* Level::GetObject(Coordinates coords)
{
	if(!IsCoordinatesValid(coords))
		return nullptr;

	return terrainObjects[coords.y * w + coords.x];
}

void Level::SetObject(Coordinates coords, Object* obj)
{
	if(!IsCoordinatesValid(coords))
		return;

	Object* oldObj = terrainObjects[coords.y * w + coords.x];
	if(oldObj && !oldObj->IsAlive() && obj != oldObj)
		oldObj->SetLife(-1);

	terrainObjects[coords.y * w + coords.x] = obj;
	terrainUsed[coords.y * w + coords.x] = (obj && obj->IsAlive());

	wasChanged = true;
}

void Level::AddOneShotSprite(std::string name, Coordinates coords)
{
	oneShotSprites.push_back(Sprite(name, coords.ToPosition()));
}

bool Level::IsCoordinatesValid(Coordinates coords)
{
	if(coords.x < 0 || coords.y < 0)
		return false;

	if((unsigned)coords.x >= w || (unsigned)coords.y >= h)
		return false;

	return true;
}

bool Level::IsTerrainUsed(Coordinates coords)
{
	if(!IsCoordinatesValid(coords)) // -____-"
	{
		//printf("Invalid coords~! %dx%d\n", coords.x, coords.y);
		return true;
	}

	if(terrainUsed[coords.y * w + coords.x])
	{
		//printf("Terrain used: %dx%d\n", coords.x, coords.y);
		return true;
	}

	Object* obj = terrainObjects[coords.y * w + coords.x];
	if(obj && obj->IsAlive())
	{
		//printf("Terrain used by obj: %dx%d\n", coords.x, coords.y);
		return true;
	}

	return false;
}

void Level::UpdateConnections()
{
	std::set<Coordinates> visitedCoords;

	for(auto obj : objects)
	{
		if(obj->GetObjectType() != Object::Type::Building)
			continue;

		Building::Building* build = (Building::Building*)obj;
		build->SetGrounded(false);
		build->SetPowered(false);
	}

	//printf("===== Updating connections =====\n");
	for(auto obj : objects)
	{
		if(obj->GetObjectType() != Object::Type::Building)
			continue;

		Building::Building* build = (Building::Building*)obj;
		if(build->GetBuildingType() != Building::Building::Type::Track) // must-have :f
			continue;

		bool grounded = false;
		bool powered = false;

		std::queue<Coordinates> toVisit;

		auto csize = build->GetCoordinatesSize();
		for(unsigned y = 0; y < csize.h; y++)
		{
			for(unsigned x = 0; x < csize.w; x++)
				toVisit.push(build->GetCoordinates() + Coordinates(x, y)); // is it needed? :v
		}

		//printf("  Checking from: %dx%d: %s\n", build->GetCoordinates().x, build->GetCoordinates().y, build->GetSprDefName().c_str());

		std::list<Building::Building*> trackPath;

		while(!toVisit.empty())
		{
			Coordinates coords = toVisit.front();
			toVisit.pop();

			Object* obj = GetObject(coords);
			if(!obj || !obj->IsAlive() || obj->GetObjectType() != Object::Type::Building)
				continue;

			Building::Building* building = (Building::Building*)GetObject(coords);

			if(visitedCoords.find(coords) != visitedCoords.end())
				continue;

			//printf("    Visiting: %dx%d: %s\n", coords.x, coords.y, building->GetSprDefName().c_str());

			visitedCoords.insert(coords);
			trackPath.push_back(building);

			// TODO fix checking tracks
			if(building->GetBuildingType() != building->Type::Other)
			{
				toVisit.push(coords + Coordinates( 0, -1));
				if(building->GetBuildingType() != building->Type::Power
						&& building->GetBuildingType() != building->Type::Ground)
				{
					toVisit.push(coords + Coordinates(-1,  0));
					toVisit.push(coords + Coordinates( 1,  0));

					toVisit.push(coords + Coordinates( 0,  1));
				}
			}

			Object* objBelow = GetObject(coords + Coordinates(0, 1));
			if(!objBelow || !objBelow->IsAlive() || objBelow->GetObjectType() != Object::Type::Building)
				continue;

			if(obj == objBelow)
				continue;

			auto buildBelow = (Building::Building*)objBelow;
			if(buildBelow->GetBuildingType() == buildBelow->Type::Ground)
			{
				grounded = true;
				//printf("      Is grounded~!\n");
			}

			if(buildBelow->GetBuildingType() == buildBelow->Type::Power)
			{
				powered = true;
				//printf("      Is powered~!\n");
			}
		}

		for(auto building : trackPath)
		{
			//printf("Updating %dx%d -> %d %d\n", building->GetCoordinates().x, building->GetCoordinates().y,
			//		grounded, powered);

			if(grounded)
				building->SetGrounded(grounded);
			if(powered)
				building->SetPowered(powered);
		}
	}
}
