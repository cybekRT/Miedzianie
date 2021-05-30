/*
 * GameOver.cpp
 *
 *  Created on: 23 May 2021
 *      Author: cybek
 */

#include"main.hpp"
#include"GameOver.hpp"
#include"Menu.hpp"

GameOver::GameOver(bool success)
{
	if(success)
		img = LoadIMG("gameover_win.png");
	else
		img = LoadIMG("gameover_lose.png");
}

GameOver::~GameOver()
{

}

void GameOver::Update(SDL_Event& ev)
{
	if(ev.type == SDL_KEYDOWN || ev.type == SDL_MOUSEBUTTONDOWN)
		SetState(new Menu());
}

void GameOver::Draw(SDL_Surface* buffer)
{
	DrawSurface(img, buffer, Position());
}
