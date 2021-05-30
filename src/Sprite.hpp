#pragma once
/*
 * Sprite.hpp
 *
 *  Created on: 17 May 2021
 *      Author: cybek
 */

#include<SDL/SDL.h>
#include<string>
#include<vector>
#include<map>
#include"Position.hpp"
#include"Size.hpp"

class SpriteInfo
{
	private:
		SDL_Surface* srf;
		Size size;
		unsigned delay;
		bool repeat;
		std::vector<Position> frames;

		static std::map<std::string, SpriteInfo*> sprites;

	private:
		SpriteInfo(std::string name);

	public:
		static SpriteInfo* Get(std::string name);
		static bool Exist(std::string name);

	friend class Sprite;
};

class Sprite
{
	protected:
		SpriteInfo* info;

		Position pos;
		unsigned timer;
		unsigned frame;
		bool finished;

	public:
		Sprite(std::string name, Position pos, unsigned frame = 0);

		void Update(unsigned delta);
		void Draw(SDL_Surface* buffer, Position camera);

		unsigned GetFrame();
		void SetFrame(unsigned frame);

		bool IsFinished();
		void SetPosition(Position pos);
};
