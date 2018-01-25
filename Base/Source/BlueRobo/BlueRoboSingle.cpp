#include "BlueRoboSingle.h"

BlueRoboSingleton* BlueRoboSingleton::s_instance = NULL;

BlueRoboSingleton::BlueRoboSingleton()
{
}

BlueRoboSingleton::~BlueRoboSingleton()
{
}

BlueRoboSingleton* BlueRoboSingleton::GetInstance()
{
	if (!s_instance)
	{
		s_instance = new BlueRoboSingleton;
	}
	return s_instance;
}