#pragma once
#ifndef ENEMY_CHASE_H
#define ENEMY_CHASE_H
#include "EnemyStatesBase.h"

class EnemyChase : public EnemyStatesBase
{
public:
	EnemyChase();
	virtual ~EnemyChase();

	void Execute();
	void Enter();
	void Exit(EnemyStatesBase* state);
};

#endif // ENEMY_CHASE_H
