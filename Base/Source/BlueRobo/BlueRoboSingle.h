#ifndef BLUEROBO_SINGLETON
#include <vector>
#include "BlueRobo.h"

class BlueRoboSingleton
{
private:
	static BlueRoboSingleton* s_instance;
public:
	static BlueRoboSingleton* GetInstance();
	BlueRoboSingleton();
	~BlueRoboSingleton();
	std::vector<CBlueRobo*> BlueRobos;
};

#endif