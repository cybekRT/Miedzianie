/*
 * Sprite.cpp
 *
 *  Created on: 17 May 2021
 *      Author: cybek
 */

#include<fstream>
#include"main.hpp"
#include"Sprite.hpp"

/******************** SpriteInfo ********************/

//std::string spritesPaths[(unsigned)SpriteType::Count] = {
//	[(unsigned)SpriteType::Test] = "test"
//};
//
//std::vector<SpriteInfo> SpriteInfo::sprites;

std::map<std::string, SpriteInfo*> SpriteInfo::sprites;

SpriteInfo::SpriteInfo(std::string name) : srf(nullptr), delay(0), repeat(false)
{
	srf = LoadIMG(name + ".png");
	if(!srf)
		throw "Sprite oopsie srf: " + name;

	std::ifstream f("data/" + name + ".spr");
	if(!f)
	{
		printf("No sprite info: %s.spr\n", name.c_str());

		size.w = srf->w;
		size.h = srf->h;

		printf("Size: %dx%d\n", size.w, size.h);

		Position pos(0, 0);
		frames.push_back(pos);
		delay = 0;
		repeat = false;

		return;
	}

	f >> size.w >> size.h;
	f >> delay >> repeat;

	while(!f.eof())
	{
		Position pos;
		f >> pos.x >> pos.y;
		if(f.fail())
			break;

		frames.push_back(pos);
	}

	f.close();
}

SpriteInfo* SpriteInfo::Get(std::string name)
{
	auto itr = sprites.find(name);
	if(itr != sprites.end())
	{
		return itr->second;
	}

	SpriteInfo* info = new SpriteInfo(name);
	if(info)
	{
		sprites[name] = info;
	}

	return info;
}

bool SpriteInfo::Exist(std::string name)
{
	auto itr = sprites.find(name);
	return itr != sprites.end();
}

/******************** Sprite ********************/

//Sprite::Sprite(SpriteType type, Position pos) : type(type), pos(pos), timer(0), frame(0), finished(false)
//{
//
//}

Sprite::Sprite(std::string name, Position pos, unsigned frame) :
		pos(pos), timer(0), frame(frame), finished(false)
{
	info = SpriteInfo::Get(name);
}

void Sprite::Update(unsigned delta)
{
	//SpriteInfo& info = SpriteInfo::sprites[(unsigned)type];

	timer += delta;

	//printf("Sprite timer: %d/%d\n", timer, info.delay);

	if(info->delay > 0 && timer >= info->delay)
	{
		timer -= info->delay;
		frame++;

		if(frame >= info->frames.size())
		{
			if(info->repeat)
				frame = 0;
			else
			{
				frame--;
				finished = true;
			}
		}
	}
}

void Sprite::Draw(SDL_Surface* buffer, Position camera)
{
	if(finished)
		return;

	//SpriteInfo& info = SpriteInfo::sprites[(unsigned)type];

	DrawSurface(info->srf, info->frames[frame], info->size, buffer, pos - camera);
}

unsigned Sprite::GetFrame()
{
	return frame;
}

void Sprite::SetFrame(unsigned frame)
{
	this->frame = frame;
}

bool Sprite::IsFinished()
{
	return finished;
}

void Sprite::SetPosition(Position pos)
{
	this->pos = pos;
}
