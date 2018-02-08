#include "EnemyPatrol.h"
#include "EnemyChase.h"
#include <iostream>
using namespace std;

EnemyPatrol::EnemyPatrol()
{

}

EnemyPatrol::~EnemyPatrol()
{

}

void EnemyPatrol::Enter()
{
	cout << "Enter Patrol!" << endl;
}

void EnemyPatrol::Execute()
{
	//cout << "Patrolling!" << endl;
	//_theEnemy.ChangeState(new EnemyChase());
}

void EnemyPatrol::Exit(EnemyStatesBase* state)
{
	delete this;
}