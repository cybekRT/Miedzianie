/*
 * AntiPiracy.cpp
 *
 *  Created on: 22 May 2021
 *      Author: cybek
 */

#include"main.hpp"
#include"Menu.hpp"
#include"GameIntro.hpp"
#include"AntiPiracy.hpp"

AntiPiracy::CodeInfo AntiPiracy::codes[] =
{
		{ .code = "kghm",
				.paragraph = 1,
				.line = 2,
				.word = 2
		},
		{ .code = "gatunek",
				.paragraph = 2,
				.line = 1,
				.word = 5
		},
		{ .code = "miedzjam",
				.paragraph = 1,
				.line = 1,
				.word = 5
		},
		{ .code = "roboty",
				.paragraph = 2,
				.line = 4,
				.word = 2
		},
		{ .code = "elektrony",
				.paragraph = 3,
				.line = 2,
				.word = 4
		}
};

AntiPiracy::AntiPiracy() :
		fontColor(SDL_Color{.r = 255, .g = 0, .b = 0}),
		bg(nullptr),
		hintSrf(nullptr),
		codeSrf(nullptr),
		retriesSrf(nullptr),
		retries(3),

		posCode(20, 276),
		posHint(20, 494),
		posRetries(746, 42),
		posWord(124, 39),
		posLine(180, 67),
		posParagraph(208, 95),

		srfWord(nullptr),
		srfLine(nullptr),
		srfParagraph(nullptr)
{
	font = TTF_OpenFont("data/DejaVuSansMono.ttf", 18);
	bg = LoadIMG("antipiracy_bg.png");

	codeIndex = rand() % (sizeof(codes) / sizeof(CodeInfo));
	auto& codeInfo = codes[codeIndex];

	srfWord = TTF_RenderText_Solid(font, std::to_string(codeInfo.word).c_str(), fontColor);
	srfLine = TTF_RenderText_Solid(font, std::to_string(codeInfo.line).c_str(), fontColor);
	srfParagraph = TTF_RenderText_Solid(font, std::to_string(codeInfo.paragraph).c_str(), fontColor);

	auto hint = "Hint: " + codeInfo.code;
	hintSrf = TTF_RenderText_Solid(font, hint.c_str(), fontColor);

	UpdateCode();
}

AntiPiracy::~AntiPiracy()
{
	SDL_FreeSurface(srfWord);
	SDL_FreeSurface(srfLine);
	SDL_FreeSurface(srfParagraph);
	SDL_FreeSurface(retriesSrf);
	SDL_FreeSurface(hintSrf);
	SDL_FreeSurface(codeSrf);
	SDL_FreeSurface(bg);
	TTF_CloseFont(font);
}

void AntiPiracy::Update(unsigned delta)
{

}

void AntiPiracy::Update(SDL_Event& ev)
{
	if(ev.type == SDL_KEYDOWN)
	{
		if(ev.key.keysym.sym == SDLK_BACKSPACE)
		{
			printf("Backspace~!\n");
			if(code.length() > 0)
			{
				code.pop_back();
				UpdateCode();
			}
		}
		else if(ev.key.keysym.sym == SDLK_RETURN)
		{
			VerifyCode();
		}
		else
		{
			auto c = tolower((char)ev.key.keysym.sym); //ev.key.keysym.unicode;
			if(c < 'a' || c > 'z')
				return;

			if(code.length() < 16)
				code += c;

			printf("Current code: %s\n", code.c_str());
			UpdateCode();
		}
	}
}

void AntiPiracy::Draw(SDL_Surface* buffer)
{
	DrawSurface(bg, buffer, Position(0, 0));

	DrawSurface(srfWord, buffer, posWord);
	DrawSurface(srfLine, buffer, posLine);
	DrawSurface(srfParagraph, buffer, posParagraph);

	DrawSurface(retriesSrf, buffer, posRetries);

	DrawSurface(codeSrf, buffer, posCode);

	DrawSurface(hintSrf, buffer, posHint);
}

void AntiPiracy::UpdateCode()
{
	if(codeSrf)
		SDL_FreeSurface(codeSrf);

	codeSrf = TTF_RenderText_Solid(font, code.c_str(), fontColor);

	if(retriesSrf)
		SDL_FreeSurface(retriesSrf);
	auto retriesMsg = std::to_string(retries);
	retriesSrf = TTF_RenderText_Solid(font, retriesMsg.c_str(), fontColor);
}

void AntiPiracy::VerifyCode()
{
	if(code == codes[codeIndex].code)
	{
		SetState(new GameIntro());
	}
	else
	{
		retries--;
		if(retries == 0)
		{
			SetState(new Menu());
		}
		else
		{
			code.clear();
			UpdateCode();
		}
	}
}
