#ifndef REDROBO_SINGLETON
#include <vector>
#include "RedRobo.h"

class RedRoboSingleton
{
private:
	static RedRoboSingleton* s_instance;
public:
	static RedRoboSingleton* GetInstance();
	RedRoboSingleton();
	~RedRoboSingleton();
	std::vector<CRedRobo*> RedRobos;
};

#endif#pragma once
