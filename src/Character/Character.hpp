#pragma once
/*
 * Character.hpp
 *
 *  Created on: 21 May 2021
 *      Author: cybek
 */

#include<queue>
#include"Object.hpp"

namespace Character
{
	class Character : public Object
	{
		public:
			enum class Type
			{
				Player,
				Enemy
			};

			struct Abilities
			{
				bool canAttack;
				bool canGather;
			};

		protected:
			Type type;
			Abilities abilities;

			std::queue<Coordinates> movementCoords;
			unsigned movementSpeed;
			unsigned movementDeltaCounter;

			Object* attackedObject;
			unsigned attackSpeed;
			unsigned attackDeltaCounter;
			unsigned attackPower;

			Object* gatheredObject;
			unsigned gatherDelay;
			unsigned gatherSpeed;
			unsigned gatherDeltaCounter;

			std::string soundAttack;

		public:
			Character(Character::Type type, Abilities abilities, Coordinates coords, std::string sprName, int life);
			virtual ~Character();

			void Update(unsigned delta) override;
			void UpdateMovement();
			void UpdateAttack();
			void UpdateGather();

			Type GetCharacterType() { return type; }
			bool CanAttack() { return abilities.canAttack; }
			bool CanGather() { return abilities.canGather; }

			void GoTo(Coordinates dstCoords, bool attackingOrGathering = false);
			void Attack(Object* obj);
			void Gather(Object* obj);
	};
}
