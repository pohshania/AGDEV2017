#ifndef BLUEROBO_SINGLETON
#include <vector>
#include "Enemy.h"

class EnemySingleton
{
private:
	static EnemySingleton* s_instance;
public:
	static EnemySingleton* GetInstance();
	EnemySingleton();
	~EnemySingleton();
	std::vector<CEnemy*> Enemys;
};

#endif
