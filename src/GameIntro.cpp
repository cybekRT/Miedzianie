/*
 * GameIntro.cpp
 *
 *  Created on: 23 May 2021
 *      Author: cybek
 */

#include"main.hpp"
#include"Game.hpp"
#include"GameIntro.hpp"

GameIntro::GameIntro() : offset(0), offsetTimer(0)
{
	img = LoadIMG("gameintro_text.png");
}

GameIntro::~GameIntro()
{
	SDL_FreeSurface(img);
}

void GameIntro::Update(unsigned delta)
{
	offsetTimer += delta;

	unsigned reqDelta = 30;
	while(offsetTimer > reqDelta)
	{
		offsetTimer -= reqDelta;

		offset += 2;
	}

	//printf("%d > %d\n", offset, img->h + 600);

	if(offset > img->h + 600)
		Finish();
}

void GameIntro::Update(SDL_Event& ev)
{
	if(ev.type == SDL_MOUSEBUTTONDOWN || ev.type == SDL_KEYDOWN)
		Finish();
}

void GameIntro::Draw(SDL_Surface* buffer)
{
	SDL_FillRect(buffer, nullptr, SDL_MapRGB(buffer->format, 0, 0, 0));
	DrawSurface(img, buffer, Position(0, buffer->h - offset));
}

void GameIntro::Finish()
{
	SetState(new Game());
}
