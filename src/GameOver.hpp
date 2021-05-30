#pragma once
/*
 * GameOver.hpp
 *
 *  Created on: 23 May 2021
 *      Author: cybek
 */

#include"State.hpp"

class GameOver : public State
{
	protected:
		SDL_Surface* img;

	public:
		GameOver(bool success);
		~GameOver();

		void Update(SDL_Event& ev) override;
		void Draw(SDL_Surface* buffer) override;
};
