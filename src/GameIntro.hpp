#pragma once
/*
 * GameIntro.hpp
 *
 *  Created on: 23 May 2021
 *      Author: cybek
 */

#include"State.hpp"

class GameIntro : public State
{
	protected:
		SDL_Surface* img;
		int offset;
		unsigned offsetTimer;

	public:
		GameIntro();
		~GameIntro();

		void Update(unsigned delta) override;
		void Update(SDL_Event& ev) override;
		void Draw(SDL_Surface* buffer) override;

		void Finish();
};
