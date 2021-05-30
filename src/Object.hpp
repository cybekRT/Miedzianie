#pragma once
/*
 * Object.hpp
 *
 *  Created on: 18 May 2021
 *      Author: cybek
 */

#include"Position.hpp"
#include"Sprite.hpp"

class Object
{
	public:
		enum class Type
		{
			Invalid,
			Building, // -_-"
			Character,
		};

	protected:
		Type objType;
		Position pos;
		Size size;
		Sprite spr;
		std::string sprDefName;
		int life;
		int maxLife;

		std::string soundDie;

	public:
		Object(Type objType, Position pos, Size size, std::string sprName, int life);
		virtual ~Object();

		virtual void Update(unsigned delta);
		virtual void Update(SDL_Event& ev);
		virtual void UpdateOnLevelChange();
		virtual void Draw(SDL_Surface* buffer, Position camera);

		Type GetObjectType() { return objType; }
		bool IsAlive() { return life > 0; }

		int GetLife() { return life; }
		int GetMaxLife() { return maxLife; }
		void SetLife(int life, bool keepSprite = false);
		void DealDamage(int dmg);

		Position GetPosition() { return pos; }
		Coordinates GetCoordinates() { return Coordinates(pos); }

		Size GetSize() { return size; }
		CoordinatesSize GetCoordinatesSize() { return CoordinatesSize(size); }

		std::string GetSprDefName() { return sprDefName; }
};

