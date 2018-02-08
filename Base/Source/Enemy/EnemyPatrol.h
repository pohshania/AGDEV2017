#pragma once
#ifndef ENEMY_PATROL_H
#define ENEMY_PATROL_H
#include "EnemyStatesBase.h"

class EnemyPatrol : public EnemyStatesBase
{
public:
	EnemyPatrol();
	virtual ~EnemyPatrol();

	void Execute();
	void Enter();
	void Exit(EnemyStatesBase* state);

protected:
};

#endif // ENEMY_PATROL_H
