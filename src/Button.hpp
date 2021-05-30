#pragma once
/*
 * Button.hpp
 *
 *  Created on: 23 May 2021
 *      Author: cybek
 */

#include<SDL/SDL.h>
#include"Sprite.hpp"

class Button
{
	protected:
		Sprite spr;
		SDL_Surface* icon;
		Position pos;
		Size size;
		Position mouseOffset;
		bool active;
		bool switchOnOff;
		bool enabled;

	public:
		Button(Position pos, Size size, Position mouseOffset, std::string iconName, bool switchOnOff = false);
		~Button();

		void Update(SDL_Event& ev);
		void Draw(SDL_Surface* buffer);

		bool IsActive() { return active; }
		void Activate(bool sendEvent = false);
		void Deactivate(bool sendEvent = false);

		void SetEnabled(bool enabled);
		bool IsEnabled() { return enabled; }

		void SetMouseOffset(Position offset) { mouseOffset = offset; }

	protected:
		void SendEvent();
};
