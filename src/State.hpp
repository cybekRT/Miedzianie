#pragma once
/*
 * State.hpp
 *
 *  Created on: 22 May 2021
 *      Author: cybek
 */

#include<SDL/SDL.h>

class State
{
	public:
		State();
		virtual ~State();

		virtual void Update(unsigned delta);
		virtual void Update(SDL_Event& ev);
		virtual void Draw(SDL_Surface* buffer);
};
