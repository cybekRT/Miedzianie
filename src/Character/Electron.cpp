/*
 * Electron.cpp
 *
 *  Created on: 21 May 2021
 *      Author: cybek
 */

#include"Electron.hpp"

namespace Character
{
	Electron::Electron(Coordinates coords) :
			Character(Character::Type::Player,
			Character::Abilities { .canAttack = true, .canGather = true },
			coords, "electron", 10)
	{
		soundAttack = "electron_attack.ogg";
	}

	Electron::~Electron()
	{

	}
}
