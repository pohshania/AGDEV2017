#include "EnemySingleton.h"

EnemySingleton* EnemySingleton::s_instance = NULL;

EnemySingleton::EnemySingleton()
{
}

EnemySingleton::~EnemySingleton()
{
}

EnemySingleton* EnemySingleton::GetInstance()
{
	if (!s_instance)
	{
		s_instance = new EnemySingleton;
	}
	return s_instance;
}