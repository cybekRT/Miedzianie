/*
 * Intro.cpp
 *
 *  Created on: 22 May 2021
 *      Author: cybek
 */

#include"main.hpp"
#include"Intro.hpp"
#include"Menu.hpp"

Intro::Intro() : logoCounter(0)
{
	logo = LoadIMG("kghm_small.png");

	int w, h;
	GetScreenSize(&w, &h);

	bg = CreateSurface(w, h);
	SDL_FillRect(bg, nullptr, SDL_MapRGB(bg->format, 0, 0, 0));

	SetMusic("music_menu.mp3");
}

Intro::~Intro()
{
	SDL_FreeSurface(bg);
	SDL_FreeSurface(logo);
}

void Intro::Update(unsigned delta)
{
	logoCounter += delta;
	if(logoCounter < 500)
	{
		SDL_SetAlpha(bg, SDL_SRCALPHA, 255);
	}
	else if(logoCounter < 1500)
	{
		SDL_SetAlpha(bg, SDL_SRCALPHA, 255 - (logoCounter - 500) / 4);
	}
	else if(logoCounter > 5500)
	{
		Finish();
	}
	else if(logoCounter > 4500)
	{
		SDL_SetAlpha(bg, SDL_SRCALPHA, (logoCounter - 4500) / 4);
	}
	else
	{
		SDL_SetAlpha(bg, SDL_SRCALPHA, 0);
	}
}

void Intro::Update(SDL_Event& ev)
{
	if(ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE)
		Finish();
}

void Intro::Draw(SDL_Surface* buffer)
{
	DrawSurface(logo, buffer, Position(0, 0));
	DrawSurface(bg, buffer, Position(0, 0));
}

void Intro::Finish()
{
	SetState(new Menu(), true);
}
