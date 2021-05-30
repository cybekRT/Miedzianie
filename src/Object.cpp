/*
 * Object.cpp
 *
 *  Created on: 18 May 2021
 *      Author: cybek
 */

#include"Object.hpp"
#include"Level.hpp"
#include"main.hpp"

Object::Object(Object::Type objType, Position pos, Size size, std::string sprName, int life) :
		objType(objType), pos(pos), size(size), spr(sprName, pos), sprDefName(sprName),
		life(life), maxLife(life)
{
	/*auto lvl = game->GetLevel();

	CoordinatesSize csize(size);
	for(unsigned y = 0; y < csize.h; y++) // -_-"
	{
		for(unsigned x = 0; x < csize.w; x++)
		{
			Coordinates coords = Coordinates(pos) + Coordinates(x, y);
			lvl->SetObject(coords, this);
		}
	}

	lvl->AddObject(this);*/
}

Object::~Object()
{
	printf("### WTF~! ###\n"); for(;;);
}

void Object::Update(unsigned delta)
{

}

void Object::Update(SDL_Event& ev)
{

}

void Object::UpdateOnLevelChange()
{

}

void Object::Draw(SDL_Surface* buffer, Position camera)
{
	spr.SetPosition(pos);
	spr.Draw(buffer, camera);

	if(life > 0 && life < maxLife)
	{
		int frame = life * 4 / maxLife;
		Sprite("hp_bar", pos, frame).Draw(buffer, camera);
	}
}

void Object::SetLife(int life, bool keepSprite)
{
	this->life = life;

	if(life <= 0)
	{
		auto lvl = game->GetLevel();

		CoordinatesSize csize(size);
		for(unsigned y = 0; y < csize.h; y++) // -_-"
		{
			for(unsigned x = 0; x < csize.w; x++)
			{
				Coordinates coords = Coordinates(pos) + Coordinates(x, y);
				if(lvl->GetObject(coords) == this) // just in case...
					lvl->SetObject(coords, this);
			}
		}

		if(!keepSprite)
		{
			if(SpriteInfo::Exist(sprDefName + "_dead"))
				spr = Sprite(sprDefName + "_dead", pos);
			else
				spr = Sprite("global_dead", pos);
		}
	}
}

void Object::DealDamage(int dmg)
{
	if(life <= 0)
		return;

	if(life > dmg)
		SetLife(life - dmg);
	else
	{
		PlaySound(soundDie);
		SetLife(0);
	}

	auto csize = CoordinatesSize(size);
	for(unsigned y = 0; y < csize.h; y++)
	{
		for(unsigned x = 0; x < csize.w; x++)
		{
			game->GetLevel()->AddOneShotSprite("spark", Coordinates(pos) + Coordinates(x, y));
		}
	}
}
