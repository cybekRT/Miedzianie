#pragma once
/*
 * AntiPiracy.hpp
 *
 *  Created on: 22 May 2021
 *      Author: cybek
 */

#include<string>
#include<SDL/SDL_ttf.h>
#include"State.hpp"

class AntiPiracy : public State
{
	protected:
		struct CodeInfo
		{
			std::string code;
			unsigned paragraph;
			unsigned line;
			unsigned word;
		};

		TTF_Font* font;
		SDL_Color fontColor;
		SDL_Surface* bg;
		SDL_Surface* hintSrf;

		std::string code;
		SDL_Surface* codeSrf;
		unsigned codeIndex;
		static CodeInfo codes[];

		unsigned retries;
		SDL_Surface* retriesSrf;

		Position posCode;
		Position posHint;
		Position posRetries;

		Position posWord;
		Position posLine;
		Position posParagraph;

		SDL_Surface* srfWord;
		SDL_Surface* srfLine;
		SDL_Surface* srfParagraph;

	public:
		AntiPiracy();
		~AntiPiracy();

		void Update(unsigned delta);
		void Update(SDL_Event& ev);
		void Draw(SDL_Surface* buffer);

		void UpdateCode();
		void VerifyCode();
};
