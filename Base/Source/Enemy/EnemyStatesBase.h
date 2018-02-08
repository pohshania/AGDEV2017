#pragma once
#ifndef ENEMY_STATES_BASE_H
#define ENEMY_STATES_BASE_H
#include "Enemy.h"
#include "../PlayerInfo/PlayerInfo.h"

class EnemyStatesBase
{
public:
	enum ENEMY_STATES
	{
		E_PATROL = 0,
		E_CHASE,
		E_ATTACK,
		E_RETURN,
	};

	EnemyStatesBase();
	virtual ~EnemyStatesBase();

	virtual void Enter() = 0;
	virtual void Execute() = 0;
	virtual void Exit(EnemyStatesBase* State) = 0;
};

#endif // ENEMY_STATES_BASE_H