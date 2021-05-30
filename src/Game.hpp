#pragma once
/*
 * Game.hpp
 *
 *  Created on: 17 May 2021
 *      Author: cybek
 */

#include<list>
#include"State.hpp"
#include"Level.hpp"
#include"Position.hpp"
#include"Sprite.hpp"
#include"Button.hpp"

class Game : public State
{
	private:
		enum class MoveType
		{
			Move,
			Attack,
			Gather
		};

		enum class BuildType
		{
			None = 0,
			Track,
			Power,
			Ground,
			Electron
		};

		Level* lvl;
		Position camera;

		SDL_Surface* hudSrf;

		SDL_Color copperBarColor;
		Position copperBarPos;
		Size copperBarSize;

		SDL_Color hpBarColor;
		Position hpBarPos;
		Size hpBarSize;

		Button buttonMove;
		Button buttonAttack;
		Button buttonGather;
		MoveType moveType;

		Button buttonBuildTrack;
		Button buttonBuildPower;
		Button buttonBuildGround;
		Button buttonBuildElectron;

		SDL_Surface* lvlRenderTarget;
		SDL_Surface* hudRenderTarget;
		SDL_Surface* selectionSrf;

		Position mouse;

		unsigned copper;
		unsigned copperLimit;
		Object* selectedObj;
		Position selectedObjPortraitPos;

		unsigned gameOverTimer;

		BuildType buildType;
		static unsigned buildCosts[];
		SDL_Surface* buildSelectionFree;
		SDL_Surface* buildSelectionUsed;

	public:
		Game();
		~Game();

		void Update(unsigned delta);
		void Update(SDL_Event& ev);
		void Draw(SDL_Surface* buffer);

		Level* GetLevel();

		unsigned GetCopper() { return copper; }
		void AddCopper(unsigned amount);
};

extern Game* game;
