#pragma once
/*
 * main.hpp
 *
 *  Created on: 17 May 2021
 *      Author: cybek
 */

#include<string>
#include<SDL/SDL.h>
#include"Game.hpp"
#include"State.hpp"

#include"Position.hpp"
#include"Size.hpp"

void GetScreenSize(int* w, int* h);
SDL_Surface* CreateSurface(int w, int h);
SDL_Surface* LoadIMG(std::string path);
void DrawSurface(SDL_Surface* srcSrf, SDL_Surface* dstSrf, Position dstPos);
void DrawSurface(SDL_Surface* srcSrf, Position srcPos, Size srcSize, SDL_Surface* dstSrf, Position dstPos);

void PlaySound(std::string soundName);
void SetMusic(std::string musicName, int volume = -1);

void SetState(State* state, bool withoutTransition = false);
