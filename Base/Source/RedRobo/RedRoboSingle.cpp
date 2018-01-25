#include "RedRoboSingle.h"

RedRoboSingleton* RedRoboSingleton::s_instance = NULL;

RedRoboSingleton::RedRoboSingleton()
{
}

RedRoboSingleton::~RedRoboSingleton()
{
}

RedRoboSingleton* RedRoboSingleton::GetInstance()
{
	if (!s_instance)
	{
		s_instance = new RedRoboSingleton;
	}
	return s_instance;
}