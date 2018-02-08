#include "EnemyStatesBase.h"
#include <iostream>
using std::cout;

EnemyStatesBase::EnemyStatesBase()
{

}

EnemyStatesBase::~EnemyStatesBase()
{

}

void EnemyStatesBase::Enter()
{
	cout << "Base!";
}

void EnemyStatesBase::Execute()
{

}

void EnemyStatesBase::Exit(EnemyStatesBase* state)
{

}

