#pragma once
/*
 * Menu.hpp
 *
 *  Created on: 22 May 2021
 *      Author: cybek
 */

#include"State.hpp"

class Menu : public State
{
	protected:
		static bool musicFromIntro;
		SDL_Surface *bg;
		SDL_Surface *buttonPlay;
		SDL_Surface *buttonExit;
		Position buttonPlayPos;
		Position buttonExitPos;

	public:
		Menu();
		~Menu();

		void Update(unsigned delta);
		void Update(SDL_Event& ev);
		void Draw(SDL_Surface* buffer);
};
