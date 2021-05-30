/*
 * Button.cpp
 *
 *  Created on: 23 May 2021
 *      Author: cybek
 */

#include"main.hpp"
#include"Button.hpp"

Button::Button(Position pos, Size size, Position mouseOffset, std::string iconName, bool switchOnOff) :
		spr("button", pos), pos(pos), size(size), mouseOffset(mouseOffset), active(false),
		switchOnOff(switchOnOff), enabled(true)
{
	icon = LoadIMG(iconName + "_icon.png");
}

Button::~Button()
{
	SDL_FreeSurface(icon);
}

void Button::Update(SDL_Event& ev)
{
	if(!enabled)
		return;

	if(ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT)
	{
		int x = ev.button.x - mouseOffset.x;
		int y = ev.button.y - mouseOffset.y;

		if(x >= pos.x && x < pos.x + size.w && y >= pos.y && y < pos.y + size.h)
		{
			if(switchOnOff && active)
				Deactivate(true);
			else
				Activate(true);
		}
	}
}

void Button::Draw(SDL_Surface* buffer)
{
	spr.Draw(buffer, Position());

	if(enabled)
		DrawSurface(icon, buffer, pos + Position(3, 3));
}

void Button::Activate(bool sendEvent)
{
	if(!active)
	{
		active = true;
		spr.SetFrame(1);

		if(sendEvent)
			SendEvent();
	}
}

void Button::Deactivate(bool sendEvent)
{
	if(active)
	{
		active = false;
		spr.SetFrame(0);

		if(sendEvent)
			SendEvent();
	}
}

void Button::SetEnabled(bool enabled)
{
	if(active && this->enabled && !enabled)
		Deactivate(true);

	this->enabled = enabled;
}

void Button::SendEvent()
{
	SDL_Event ev;
	ev.type = SDL_USEREVENT;
	ev.user.code = active;
	ev.user.data1 = this;
	SDL_PushEvent(&ev);

	PlaySound("click.ogg");
}
