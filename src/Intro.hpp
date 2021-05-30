#pragma once
/*
 * Intro.hpp
 *
 *  Created on: 22 May 2021
 *      Author: cybek
 */

#include"State.hpp"

class Intro : public State
{
	protected:
		SDL_Surface* logo;
		SDL_Surface* bg;
		unsigned logoCounter;

	public:
		Intro();
		~Intro();

		void Update(unsigned delta);
		void Update(SDL_Event& ev);
		void Draw(SDL_Surface* buffer);

		void Finish();
};
