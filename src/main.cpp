/*
 * main.cpp
 *
 *  Created on: 17 May 2021
 *      Author: cybek
 */

#include<string>
#include<ctime>
#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<SDL/SDL_mixer.h>
#include<SDL/SDL_ttf.h>
#include"main.hpp"
#include"Level.hpp"
#include"Intro.hpp"
#include"GameIntro.hpp"

SDL_Surface* screen;
Game* game = nullptr;

State* currentState = nullptr;

bool transition = false;
unsigned transitionTimer = 0;
SDL_Surface* transitionBuffer;
SDL_Surface* transitionBackground;
Mix_Music* music = nullptr;
std::map<std::string, Mix_Chunk*> sounds;

void GetScreenSize(int* w, int* h)
{
	if(w)
		*w = screen->w;
	if(h)
		*h = screen->h;
}

SDL_Surface* CreateSurface(int w, int h)
{
	SDL_Surface* srf = SDL_CreateRGBSurface(screen->flags, w, h, screen->format->BitsPerPixel,
			screen->format->Rmask,
			screen->format->Gmask,
			screen->format->Bmask,
			screen->format->Amask);
	SDL_FillRect(srf, nullptr, 0);

	return srf;
}

SDL_Surface* LoadIMG(std::string path)
{
	SDL_Surface* tmp = IMG_Load(("data/" + path).c_str());
	if(!tmp)
		return nullptr;

	SDL_Surface* img = SDL_DisplayFormatAlpha(tmp);
	SDL_FreeSurface(tmp);

	return img;
}

void DrawSurface(SDL_Surface* srcSrf, SDL_Surface* dstSrf, Position dstPos)
{
	if(!srcSrf)
		return;

	DrawSurface(srcSrf, Position(0, 0), Size(srcSrf->w, srcSrf->h), dstSrf, dstPos);
}

void DrawSurface(SDL_Surface* srcSrf, Position srcPos, Size srcSize, SDL_Surface* dstSrf, Position dstPos)
{
	if(!srcSrf || !dstSrf)
		return;

	SDL_Rect src, dst;

	src.x = srcPos.x;
	src.y = srcPos.y;
	src.w = srcSize.w;
	src.h = srcSize.h;

	dst.x = dstPos.x;
	dst.y = dstPos.y;
	dst.w = srcSize.w;
	dst.h = srcSize.h;

	SDL_BlitSurface(srcSrf, &src, dstSrf, &dst);
}

void PlaySound(std::string soundName)
{
	if(soundName.empty())
		return;

	Mix_Chunk* chunk = nullptr;

	auto itr = sounds.find(soundName);
	if(itr != sounds.end())
		chunk = itr->second;
	else
	{
		chunk = Mix_LoadWAV(("data/" + soundName).c_str());
		if(chunk)
			sounds[soundName] = chunk;
		else
			return;
	}

	Mix_PlayChannel(-1, chunk, 0);
}

void SetMusic(std::string musicName, int volume)
{
	if(volume < 0)
		volume = MIX_MAX_VOLUME;

	if(music)
	{
		Mix_FreeMusic(music);
		music = nullptr;
	}

	if(musicName.empty())
		return;

	music = Mix_LoadMUS(("data/" + musicName).c_str());
	if(music)
	{
		Mix_PlayMusic(music, -1);
		Mix_VolumeMusic(volume);
	}
}

void SetState(State* state, bool withoutTransition)
{
	printf("Set state as: %p\n", state);
	if(currentState)
	{
		delete currentState;

		if(!withoutTransition)
		{
			transition = true;
			DrawSurface(screen, transitionBuffer, Position(0, 0));
		}

		transitionTimer = SDL_GetTicks();
	}

	if(dynamic_cast<Game*>(state)) // huh
		game = (Game*)state;

	currentState = state;
}

void MainLoop()
{
	SDL_Event ev;
	bool quit = false;
	unsigned deltaTimer = SDL_GetTicks();
	unsigned delta = 0;
	while(!quit)
	{
		while(SDL_PollEvent(&ev))
		{
			switch(ev.type)
			{
				case SDL_QUIT:
				{
					quit = true;
					break;
				}
				case SDL_KEYDOWN:
				{
					switch(ev.key.keysym.sym)
					{
						case SDLK_F4:
						{
							if(ev.key.keysym.mod & KMOD_LALT)
								quit = true;

							break;
						}
						default:
						{
							break;
						}
					}

					break;
				}
				case SDL_MOUSEBUTTONDOWN:
				{
					break;
				}
				default:
				{
					break;
				}
			}

			//game->Update(ev);
			if(currentState && !transition)
				currentState->Update(ev);
		}

		unsigned newDeltaTimer = SDL_GetTicks();
		delta = newDeltaTimer - deltaTimer;
		if(delta < 16)
		{
			SDL_Delay(1);
			continue; // -_-"
		}
		else if(delta > 100)
		{
			printf("Game running too slow: %dms...\n", delta);
			delta = 100;
		}

		deltaTimer = newDeltaTimer;

		//game->Update(delta);

		if(currentState && !transition)
			currentState->Update(delta);

		SDL_FillRect(screen, nullptr, -1);

		//game->Draw(screen);
		//printf("State: %p\n", currentState);

		if(transition)
		{
			SDL_SetAlpha(transitionBackground, SDL_SRCALPHA, (SDL_GetTicks() - transitionTimer) / 4);

			DrawSurface(transitionBuffer, screen, Position(0, 0));
			DrawSurface(transitionBackground, screen, Position(0, 0));

			if(SDL_GetTicks() - transitionTimer >= 1000)
			{
				transition = false;
				transitionTimer = SDL_GetTicks();
			}
		}
		else if(currentState)
		{
			currentState->Draw(screen);

			if(SDL_GetTicks() - transitionTimer < 1000)
			{
				SDL_SetAlpha(transitionBackground, SDL_SRCALPHA, 255 - (SDL_GetTicks() - transitionTimer) / 4);
				DrawSurface(transitionBackground, screen, Position(0, 0));
			}
		}

		SDL_Flip(screen);

		if(!currentState && !transition)
			quit = true;
	}
}

#undef main
int main(int argc, char *argv[])
{
	//setbuf(stdout, nullptr);
	//fclose(stdout);
	srand(time(nullptr));

	/*
	 * Init SDL
	 */
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return 1;
	SDL_WM_SetCaption("Miedzianie", nullptr);
	screen = SDL_SetVideoMode(800, 600, 32, 0);
	if(!screen)
		return 2;

	//SDL_EnableKeyRepeat(1000, 100);
	SDL_EnableUNICODE(SDL_ENABLE);

	/*
	 * Init SDL_image
	 */
	if(IMG_Init(IMG_INIT_PNG) < 0)
		return 3;

	/*
	 * Init SDL_ttf
	 */
	if(TTF_Init() < 0)
		return 4;

	/*
	 * Init SDL_mixer
	 */
	if(Mix_Init(MIX_INIT_MP3) < 0)
		return 5;
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
		return 6;
	if(Mix_AllocateChannels(64) < 0)
		return 7;

	transitionBuffer = CreateSurface(screen->w, screen->h);
	transitionBackground =	CreateSurface(screen->w, screen->h);
	SDL_FillRect(transitionBackground, nullptr, SDL_MapRGB(transitionBackground->format, 0, 0, 0));

//	SetState(new Intro());

	//SetState(new AntiPiracy());
	//SetState(new Game());

	try
	{
		SetState(new Intro());
		//SetState(new AntiPiracy());
		//SetState(new GameIntro());

		printf("Game started~!\n");
		MainLoop();
	}
	catch(const std::string& str)
	{
		printf("Exception: %s\n", str.c_str());
	}
	catch(const char* str)
	{
		printf("Exception: %s\n", str);
	}

	printf("Game closed~!\n");
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}
