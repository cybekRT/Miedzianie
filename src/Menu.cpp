/*
 * Menu.cpp
 *
 *  Created on: 22 May 2021
 *      Author: cybek
 */

#include"main.hpp"
#include"Menu.hpp"
#include"AntiPiracy.hpp"
#include"Game.hpp"

bool Menu::musicFromIntro = true;

Menu::Menu() : buttonPlayPos(210, 180), buttonExitPos(210, 380)
{
	bg = LoadIMG("menu_bg.png");
	buttonPlay = LoadIMG("menu_play.png");
	buttonExit = LoadIMG("menu_exit.png");

	if(musicFromIntro)
		musicFromIntro = false;
	else
		SetMusic("music_menu.mp3");
}

Menu::~Menu()
{

}

void Menu::Update(unsigned delta)
{

}

void Menu::Update(SDL_Event& ev)
{
	if(ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT)
	{
		int mx, my;
		mx = ev.button.x;
		my = ev.button.y;

		auto checkInside = [mx, my](Position pos, Size size) -> bool {
			return mx >= pos.x && my >= pos.y && mx < pos.x + size.w && my < pos.y + size.h;
		};

		if(checkInside(buttonPlayPos, Size(buttonPlay->w, buttonPlay->h)))
		{
			PlaySound("click.ogg");
			SetState(new AntiPiracy());
		}
		if(checkInside(buttonExitPos, Size(buttonExit->w, buttonExit->h)))
		{
			PlaySound("click.ogg");
			SetState(nullptr);
		}
	}
}

void Menu::Draw(SDL_Surface* buffer)
{
	DrawSurface(bg, Position(0, 0), Size(bg->w, bg->h), buffer, Position(0, 0));
	DrawSurface(buttonPlay, Position(0, 0), Size(buttonPlay->w, buttonPlay->h), buffer, buttonPlayPos);
	DrawSurface(buttonExit, Position(0, 0), Size(buttonExit->w, buttonExit->h), buffer, buttonExitPos);
}
