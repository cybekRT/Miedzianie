/*
 * Game.cpp
 *
 *  Created on: 17 May 2021
 *      Author: cybek
 */

#include"main.hpp"
#include"Menu.hpp"
#include"Game.hpp"
#include"Sprite.hpp"
#include"GameOver.hpp"

#include "Building/Track.hpp"
#include "Building/Power.hpp"
#include "Building/Ground.hpp"
#include "Building/MCU.hpp"
#include "Building/Mine.hpp"

#include "Character/Electron.hpp"
#include "Character/Oxygen.hpp"

unsigned Game::buildCosts[] = {
	[(int)BuildType::None] = 0,
	[(int)BuildType::Track] = 2,
	[(int)BuildType::Power] = 20,
	[(int)BuildType::Ground] = 10,
	[(int)BuildType::Electron] = 10,
};

Game::Game() :
		buttonMove(Position(15, 101), Size(40, 40), Position(), "move"),
		buttonAttack(Position(80, 101), Size(40, 40), Position(), "attack"),
		buttonGather(Position(145, 101), Size(40, 40), Position(), "gather"),
		moveType(MoveType::Move),

		buttonBuildTrack(Position(15, 189), Size(40, 40), Position(), "track", true),
		buttonBuildPower(Position(15, 237), Size(40, 40), Position(), "power", true),
		buttonBuildGround(Position(15, 285), Size(40, 40), Position(), "ground", true),
		buttonBuildElectron(Position(15, 381), Size(40, 40), Position(), "electron", false),

		copper(10), copperLimit(100), selectedObj(nullptr),
		gameOverTimer(0),
		buildType(BuildType::None)
{
	lvl = new Level("level1");

	hudSrf = LoadIMG("hud.png");
	hudRenderTarget = CreateSurface(hudSrf->w, hudSrf->h);
	copperBarColor = SDL_Color { .r = 255, .g = 140, .b = 0 };
	copperBarPos = Position(140, 180);
	copperBarSize = Size(40, 400);

	hpBarColor = SDL_Color { .r = 255, .g = 0, .b = 0 };
	hpBarPos = Position(6, 6);
	hpBarSize = Size(90, 8);

	int screenW, screenH;
	GetScreenSize(&screenW, &screenH);
	lvlRenderTarget = CreateSurface(screenW - hudSrf->w, screenH);

	buttonMove.Activate();
	buttonMove.SetMouseOffset(Position(lvlRenderTarget->w, 0));
	buttonAttack.SetMouseOffset(Position(lvlRenderTarget->w, 0));
	buttonGather.SetMouseOffset(Position(lvlRenderTarget->w, 0));

	buttonBuildTrack.SetMouseOffset(Position(lvlRenderTarget->w, 0));
	buttonBuildPower.SetMouseOffset(Position(lvlRenderTarget->w, 0));
	buttonBuildGround.SetMouseOffset(Position(lvlRenderTarget->w, 0));
	buttonBuildElectron.SetMouseOffset(Position(lvlRenderTarget->w, 0));

	selectionSrf = LoadIMG("selection2.png");
	selectedObjPortraitPos = Position(130, 6);

	buildSelectionFree = LoadIMG("selection_build_free.png");
	buildSelectionUsed = LoadIMG("selection_build_used.png");

	SetMusic("music_game.mp3", 50);
}

Game::~Game()
{
	// TODO: free resources
}

void Game::Update(unsigned delta)
{
	if(selectedObj && !selectedObj->IsAlive())
		selectedObj = nullptr;

	Uint8* key = SDL_GetKeyState(nullptr);

	if(key[SDLK_LEFT])
	{
		camera.x -= 80 * delta / 100;
	}
	if(key[SDLK_RIGHT])
	{
		camera.x += 80 * delta / 100;
	}
	if(key[SDLK_UP])
	{
		camera.y -= 80 * delta / 100;
	}
	if(key[SDLK_DOWN])
	{
		camera.y += 80 * delta / 100;
	}

	if(camera.x < 0)
		camera.x = 0;
	if(camera.y < 0)
		camera.y = 0;
	if(camera.x + lvlRenderTarget->w > lvl->GetSize().w * 32)
		camera.x = lvl->GetSize().w * 32 - lvlRenderTarget->w;
	if(camera.y + lvlRenderTarget->h > lvl->GetSize().h * 32)
		camera.y = lvl->GetSize().h * 32 - lvlRenderTarget->h;

	lvl->Update(delta);

	gameOverTimer += delta;
	if(lvl->GetEnemyUnitsCount() == 0)
	{
		//printf("lvl->GetEnemyUnitsCount() == 0, %d\n", gameOverTimer);
		if(gameOverTimer > 2000)
			SetState(new GameOver(true));
	}
	else if(lvl->GetPlayerUnitsCount() == 0)
	{
		//printf("lvl->GetPlayerUnitsCount() == 0, %d\n", gameOverTimer);
		if(gameOverTimer > 2000)
			SetState(new GameOver(false));
	}
	else
	{
		//printf("%d <-> %d\n", lvl->GetEnemyUnitsCount(), lvl->GetPlayerUnitsCount());
		gameOverTimer = 0;
	}

	buttonBuildTrack.SetEnabled(copper >= buildCosts[(int)BuildType::Track]);
	buttonBuildPower.SetEnabled(copper >= buildCosts[(int)BuildType::Power]);
	buttonBuildGround.SetEnabled(copper >= buildCosts[(int)BuildType::Ground]);

	/*bool enableElectron = (selectedObj
			&& selectedObj->GetObjectType() == Object::Type::Building
			&& ((Building::Building*)selectedObj)->GetBuildingType() == Building::Building::Type::Other);*/
	auto selectedMCU = dynamic_cast<Building::MCU*>(selectedObj);
	bool enableElectron = (selectedMCU && selectedMCU->IsPowered() && selectedMCU->IsGrounded());
	buttonBuildElectron.SetEnabled(copper >= buildCosts[(int)BuildType::Electron] && enableElectron);

	//printf("BuildType: %d\n", buildType);

	if(buildType == BuildType::Electron)
	{
		auto mcu = dynamic_cast<Building::MCU*>(selectedObj);

		if(mcu)
		{
			printf("Building electron...\n");
			bool built = false;
			for(int y = -1; y < (int)mcu->GetCoordinatesSize().w + 1 && !built; y++)
			{
				for(int x = -1; x < (int)mcu->GetCoordinatesSize().w + 1 && !built; x++)
				{
					Coordinates coords = mcu->GetCoordinates() + Coordinates(x, y);

					printf("Checking coords: %dx%d\n", coords.x, coords.y);

					auto objAt = lvl->GetObject(coords);
					bool canBuild = !lvl->IsTerrainUsed(coords) || (objAt && !objAt->IsAlive());
					printf("  %d %d\n", !!objAt, canBuild);

					if(canBuild)
					{
						auto e = new Character::Electron(coords);
						lvl->AddObject(e);
						built = true;

						copper -= buildCosts[(int)BuildType::Electron];
					}
				}
			}
		}

		buttonBuildElectron.Deactivate(true);
	}
}

void Game::Update(SDL_Event& ev)
{
	lvl->Update(ev);

	switch(ev.type)
	{
		case SDL_KEYDOWN:
		{
			switch(ev.key.keysym.sym)
			{
				case SDLK_ESCAPE:
				{
					SetState(new Menu());
					break;
				}
				/*case SDLK_LEFT:
				{
					//camera.x -= 10;
					break;
				}
				case SDLK_RIGHT:
				{
					//camera.x += 10;
					break;
				}
				case SDLK_UP:
				{
					//camera.y -= 10;
					break;
				}
				case SDLK_DOWN:
				{
					//camera.y += 10;
					break;
				}*/

				case SDLK_m:
				{
					buttonMove.Activate(true);
					break;
				}
				case SDLK_a:
				{
					buttonAttack.Activate(true);
					break;
				}
				case SDLK_g:
				{
					buttonGather.Activate(true);
					break;
				}

				/*case SDLK_w:
				case SDLK_p:
				case SDLK_g:
				case SDLK_m:
				case SDLK_c:
				case SDLK_e:
				case SDLK_o:
				{
					Coordinates mcoords = Coordinates(mouse);

					if(!lvl->IsTerrainUsed(mcoords))
					{
						if(ev.key.keysym.sym == SDLK_w)
						{
							printf("Creating wall...\n");
							Building::Track* track = new Building::Track(mcoords);
						}
						else if(ev.key.keysym.sym == SDLK_p)
						{
							printf("Creating power...\n");
							Building::Power* power = new Building::Power(mcoords);
						}
						else if(ev.key.keysym.sym == SDLK_g)
						{
							printf("Creating ground...\n");
							Building::Ground* ground = new Building::Ground(mcoords);
						}
						else if(ev.key.keysym.sym == SDLK_m)
						{
							printf("Creating MCU...\n");
							Building::MCU* mcu = new Building::MCU(mcoords);
						}
						else if(ev.key.keysym.sym == SDLK_c)
						{
							printf("Creating mine...\n");
							Building::Mine* mine = new Building::Mine(mcoords);
						}
						else if(ev.key.keysym.sym == SDLK_e)
						{
							printf("Creating electron...\n");
							Character::Electron* electron = new Character::Electron(mcoords);
						}
						else if(ev.key.keysym.sym == SDLK_o)
						{
							printf("Creating oxygen...\n");
							Character::Oxygen* oxygen = new Character::Oxygen(mcoords);
						}
					}

					break;
				}*/

				default:
				{
					break;
				}
			}

			break;
		}
		case SDL_MOUSEMOTION:
		{
			mouse = Position(ev.motion.x, ev.motion.y) + camera;
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			//oneShotSprites.push_back(Sprite("test", Coordinates(Position(ev.button.x, ev.button.y)).ToPosition()));
			auto mouseCoords = Coordinates(mouse);

			if(ev.button.button == SDL_BUTTON_LEFT)
			{
				if(ev.button.x >= lvlRenderTarget->w)
					break;

				if(buildType == BuildType::None)
				{
					selectedObj = (lvl->IsTerrainUsed(mouseCoords)) ? lvl->GetObject(mouseCoords) : nullptr;
				}
				else
				{
					auto objAt = lvl->GetObject(mouseCoords);
					bool canBuild = (!lvl->IsTerrainUsed(mouseCoords) || (objAt && objAt->GetLife() <= 0));

					if(buildType == BuildType::Power)
					{
						auto coords2 = mouseCoords + Coordinates(0, 1);
						auto objAt2 = lvl->GetObject(coords2);

						canBuild = canBuild && (!lvl->IsTerrainUsed(coords2) || (objAt2 && objAt2->GetLife() <= 0));
					}

					if(canBuild)
					{
						Object* obj = nullptr;

						if(buildType == BuildType::Track)
							obj = new Building::Track(mouseCoords);
						else if(buildType == BuildType::Power)
							obj = new Building::Power(mouseCoords);
						else if(buildType == BuildType::Ground)
							obj = new Building::Ground(mouseCoords);

						lvl->AddObject(obj);

						copper -= buildCosts[(int)buildType];
					}
				}
			}
			else if(ev.button.button == SDL_BUTTON_RIGHT)
			{
				if(buildType != BuildType::None)
				{
					buttonBuildTrack.Deactivate(true);
					buttonBuildPower.Deactivate(true);
					buttonBuildGround.Deactivate(true);
					break;
				}

				if(!selectedObj)
					break;

				if(ev.button.x >= lvlRenderTarget->w)
					break;

				if(selectedObj->GetObjectType() == Object::Type::Character)
				{
					Character::Character* c = (Character::Character*)selectedObj;
					if(c->GetCharacterType() == Character::Character::Type::Enemy)
						break;
				}

				//for(auto obj : lvl->GetObjects())
				auto obj = selectedObj;
				{
					if(obj->GetObjectType() != obj->Type::Character)
						break;
						//continue;

					auto c = (Character::Character*)obj;
					Object* objAt = (lvl->IsTerrainUsed(mouseCoords)) ? lvl->GetObject(mouseCoords) : nullptr;

					//Uint8* key = SDL_GetKeyState(nullptr);
					//if(key[SDLK_LSHIFT] && objAt)
					if(moveType == MoveType::Attack)
					{
						if(objAt)
						{
							c->Attack(objAt);
							//oneShotSprites.push_back(Sprite("cursor_attack", mouseCoords.ToPosition()));
							lvl->AddOneShotSprite("cursor_attack", mouseCoords);
						}
						else
						{
							c->GoTo(mouseCoords);
							lvl->AddOneShotSprite("cursor_goto", mouseCoords);
						}

						//lvl->AddOneShotSprite("cursor_attack", mouseCoords);
					}
					//else if(key[SDLK_LCTRL] && objAt)
					else if(moveType == MoveType::Gather)
					{
						if(objAt)
						{
							c->Gather(objAt);
							lvl->AddOneShotSprite("cursor_gather", mouseCoords);
						}
						else
						{
							c->GoTo(mouseCoords);
							lvl->AddOneShotSprite("cursor_goto", mouseCoords);
						}
					}
					else if(moveType == MoveType::Move)
					{
						c->GoTo(mouseCoords);
						//oneShotSprites.push_back(Sprite("cursor_goto", mouseCoords.ToPosition()));
						lvl->AddOneShotSprite("cursor_goto", mouseCoords);
					}
				}
			}

			break;
		}
		case SDL_USEREVENT:
		{
			if(!ev.user.code)
			{
				if(ev.user.data1 == (void*)&buttonBuildTrack
						|| ev.user.data1 == (void*)&buttonBuildPower
						|| ev.user.data1 == (void*)&buttonBuildGround
						|| ev.user.data1 == (void*)&buttonBuildElectron)
				{
					buildType = BuildType::None;
				}

				break;
			}

			/* Movement */
			if(ev.user.data1 == (void*)&buttonMove)
			{
				moveType = MoveType::Move;
				//buttonMove.Deactivate();
				buttonAttack.Deactivate();
				buttonGather.Deactivate();
			}
			else if(ev.user.data1 == (void*)&buttonAttack)
			{
				moveType = MoveType::Attack;
				buttonMove.Deactivate();
				//buttonAttack.Deactivate();
				buttonGather.Deactivate();
			}
			else if(ev.user.data1 == (void*)&buttonGather)
			{
				moveType = MoveType::Gather;
				buttonMove.Deactivate();
				buttonAttack.Deactivate();
				//buttonGather.Deactivate();
			}
			/* Build */
			if(ev.user.data1 == (void*)&buttonBuildTrack)
			{
				buildType = BuildType::Track;
				//buttonBuildTrack.Deactivate();
				buttonBuildPower.Deactivate();
				buttonBuildGround.Deactivate();

				selectedObj = nullptr;
			}
			else if(ev.user.data1 == (void*)&buttonBuildPower)
			{
				buildType = BuildType::Power;
				buttonBuildTrack.Deactivate();
				//buttonBuildPower.Deactivate();
				buttonBuildGround.Deactivate();

				selectedObj = nullptr;
			}
			else if(ev.user.data1 == (void*)&buttonBuildGround)
			{
				buildType = BuildType::Ground;
				buttonBuildTrack.Deactivate();
				buttonBuildPower.Deactivate();
				//buttonBuildGround.Deactivate();

				selectedObj = nullptr;
			}
			else if(ev.user.data1 == (void*)&buttonBuildElectron)
			{
				buildType = BuildType::Electron;
				buttonBuildTrack.Deactivate();
				buttonBuildPower.Deactivate();
				buttonBuildGround.Deactivate();

				//selectedObj = nullptr;
			}

			break;
		}
		default:
		{
			break;
		}
	}

	buttonMove.Update(ev);
	buttonAttack.Update(ev);
	buttonGather.Update(ev);

	buttonBuildTrack.Update(ev);
	buttonBuildPower.Update(ev);
	buttonBuildGround.Update(ev);
	buttonBuildElectron.Update(ev);
}

void Game::Draw(SDL_Surface* buffer)
{
	SDL_FillRect(lvlRenderTarget, nullptr, SDL_MapRGB(lvlRenderTarget->format, 0, 0, 0));
	SDL_FillRect(hudRenderTarget, nullptr, SDL_MapRGB(hudRenderTarget->format, 0, 0, 0));

	// Draw level
	//lvl->Draw(lvlRenderTarget, camera);
	lvl->DrawTerrain(lvlRenderTarget, camera);

	if(selectedObj)
	{
		//Coordinates coords = selectedObj->GetCoordinates();
		CoordinatesSize coordsSize = selectedObj->GetCoordinatesSize();

		if(coordsSize.w == 1 && coordsSize.h == 1)
		{
			DrawSurface(selectionSrf, Position(32, 32), Size(32, 32), lvlRenderTarget,
					selectedObj->GetPosition() - camera);
		}
		else
		{
			DrawSurface(selectionSrf, Position(0, 0), Size(32, 32), lvlRenderTarget,
					selectedObj->GetPosition() - camera);
			DrawSurface(selectionSrf, Position(64, 0), Size(32, 32), lvlRenderTarget,
					selectedObj->GetPosition() + Coordinates(coordsSize.w - 1, 0).ToPosition() - camera);

			// TODO: support > 2x2

			DrawSurface(selectionSrf, Position(0, 64), Size(32, 32), lvlRenderTarget,
					selectedObj->GetPosition() + Coordinates(0, coordsSize.h - 1).ToPosition() - camera);
			DrawSurface(selectionSrf, Position(64, 64), Size(32, 32), lvlRenderTarget,
					selectedObj->GetPosition() + Coordinates(coordsSize.w - 1, coordsSize.h - 1).ToPosition() - camera);
		}
	}

	lvl->DrawObjects(lvlRenderTarget, camera);

	if(buildType != BuildType::None && buildType != BuildType::Electron)
	{
		auto coords = Coordinates(mouse);

		Object* objAt = (lvl->IsTerrainUsed(coords) ? lvl->GetObject(coords) : nullptr);
		bool terrainUsed = (lvl->IsTerrainUsed(coords) && !objAt) || (objAt && objAt->GetLife() > 0);
		DrawSurface((terrainUsed) ? buildSelectionUsed : buildSelectionFree,
				Position(32, 32), Size(32, 32), lvlRenderTarget, coords.ToPosition());

		if(buildType == BuildType::Power)
		{
			auto coords2 = coords + Coordinates(0, 1);
			Object* objAt2 = (lvl->IsTerrainUsed(coords2) ? lvl->GetObject(coords2) : nullptr);
			bool terrainUsed = (lvl->IsTerrainUsed(coords2) && !objAt2) || (objAt2 && objAt2->GetLife() > 0);
			DrawSurface((terrainUsed) ? buildSelectionUsed : buildSelectionFree,
					Position(32, 32), Size(32, 32), lvlRenderTarget, coords2.ToPosition());
		}
	}

	// Draw HUD
	DrawSurface(hudSrf, hudRenderTarget, Position(0, 0));

	// Copper bar
	int copperBarHeight = copperBarSize.h * copper / copperLimit;
	SDL_Rect copperBarRect;
	copperBarRect.x = copperBarPos.x;
	copperBarRect.y = copperBarPos.y + copperBarSize.h - copperBarHeight;
	copperBarRect.w = copperBarSize.w;
	copperBarRect.h = copperBarHeight;
	SDL_FillRect(hudRenderTarget, &copperBarRect, SDL_MapRGB(hudRenderTarget->format,
			copperBarColor.r, copperBarColor.g, copperBarColor.b));

	// Selected obj
	if(selectedObj)
	{
		// HP bar
		int hpBarWidth = hpBarSize.w * selectedObj->GetLife() / selectedObj->GetMaxLife();
		SDL_Rect hpBarRect;
		hpBarRect.x = hpBarPos.x;
		hpBarRect.y = hpBarPos.y;
		hpBarRect.w = (hpBarWidth <= hpBarSize.w) ? hpBarWidth : hpBarSize.w;
		hpBarRect.h = hpBarSize.h;
		SDL_FillRect(hudRenderTarget, &hpBarRect, SDL_MapRGB(hudRenderTarget->format,
				hpBarColor.r, hpBarColor.g, hpBarColor.b));

		// Portrait
		Size size = selectedObj->GetSize();
		auto pos = selectedObjPortraitPos + Position(32, 32) - Position(size.w / 2, size.h / 2);
		Sprite spr(selectedObj->GetSprDefName(), pos);

		spr.Draw(hudRenderTarget, Position());
	}

	// Buttons
	buttonMove.Draw(hudRenderTarget);
	buttonAttack.Draw(hudRenderTarget);
	buttonGather.Draw(hudRenderTarget);

	buttonBuildTrack.Draw(hudRenderTarget);
	buttonBuildPower.Draw(hudRenderTarget);
	buttonBuildGround.Draw(hudRenderTarget);
	buttonBuildElectron.Draw(hudRenderTarget);

	// Draw to buffer
	DrawSurface(lvlRenderTarget, buffer, Position(0, 0));
	DrawSurface(hudRenderTarget, buffer, Position(lvlRenderTarget->w, 0));
}

Level* Game::GetLevel()
{
	return lvl;
}

void Game::AddCopper(unsigned amount)
{
	copper += amount;
	if(copper > copperLimit)
		copper = copperLimit;
}
