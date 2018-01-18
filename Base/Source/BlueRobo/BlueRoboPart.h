#pragma once
#include "../GenericEntity.h"

class Mesh;

class CBlueRoboPart :
	public GenericEntity
{
public:
	CBlueRoboPart(void);
	virtual ~CBlueRoboPart();

	void Init(void);
	// Reset  instance to default
	void Reset(void);

	// Update
	void Update(double dt = 0.0333f);

	// Render
	void Render(void);
};
