#include "EnemyChase.h"
#include <iostream>
using namespace std;

EnemyChase::EnemyChase()
{

}

EnemyChase::~EnemyChase()
{

}

void EnemyChase::Enter()
{
	cout << "Enter Chase!" << endl;
}

void EnemyChase::Execute()
{
	cout << "Chasing!" << endl;
}

void EnemyChase::Exit(EnemyStatesBase* state)
{

}

