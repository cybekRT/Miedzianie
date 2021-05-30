#pragma once
/*
 * Level.hpp
 *
 *  Created on: 17 May 2021
 *      Author: cybek
 */

#include<list>
#include<string>
#include<SDL/SDL.h>
#include"Position.hpp"
#include"Object.hpp"

class Level
{
	private:
		SDL_Surface* tiles;
		SDL_Surface* freeSrf;
		SDL_Surface* usedSrf;
		unsigned* terrain;
		bool* terrainUsed;
		Object** terrainObjects;
		std::list<Object*> objects;
		std::list<Sprite> oneShotSprites;
		unsigned w;
		unsigned h;
		bool wasChanged;

		unsigned enemyUnits;
		unsigned playerUnits;

	public:
		Level(std::string path);
		~Level();

		void Update(unsigned delta);
		void Update(SDL_Event& ev);
		void Draw(SDL_Surface* buffer, Position camera);

		void DrawTerrain(SDL_Surface* buffer, Position camera);
		void DrawObjects(SDL_Surface* buffer, Position camera);

		void AddObject(Object* obj);
		void RemoveObject(Object* obj);
		Object* GetObject(Coordinates coords);
		void SetObject(Coordinates pos, Object* obj);
		std::list<Object*>& GetObjects() { return objects; }

		void AddOneShotSprite(std::string name, Coordinates coords);

		Size GetSize() { return Size(w, h); }

		bool IsCoordinatesValid(Coordinates coords); // is :v
		bool IsTerrainUsed(Coordinates coords);

		void UpdateConnections();

		unsigned GetEnemyUnitsCount() { return enemyUnits; }
		unsigned GetPlayerUnitsCount() { return playerUnits; }
};
