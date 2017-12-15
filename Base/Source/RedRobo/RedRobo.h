#pragma once
#include "../GenericEntity.h"
#include "../GroundEntity.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../SceneGraph/SceneGraph.h"
#include "../SceneGraph/SceneNode.h"
#include "RedRoboHead.h"
#include "RedRoboBottom.h"


class Mesh;

class CRedRobo :
	public GenericEntity
{
protected:
	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up;
	Vector3 maxBoundary, minBoundary;
	float directionVecX, directionVecZ, offset;
	GroundEntity* m_pTerrain;

	double m_dSpeed;
	double m_dAcceleration;

	CRedRoboHead* redRoboHead;
	CRedRoboBottom* redRoboBottom;

public:
	CRedRobo(void);
	virtual ~CRedRobo();

	void Init(float x, float y, float z);
	// Reset this player instance to default
	void Reset(void);

	// Set position
	void SetPos(const Vector3& pos);
	// Set target
	void SetTarget(const Vector3& target);
	// Set Up
	void SetUp(const Vector3& up);
	// Set the boundary for the player info
	void SetBoundary(Vector3 max, Vector3 min);
	// Set the terrain for the player info
	void SetTerrain(GroundEntity* m_pTerrain);

	// Get position
	Vector3 GetPos(void) const;
	// Get target
	Vector3 GetTarget(void) const;
	// Get Up
	Vector3 GetUp(void) const;
	// Get the terrain for the player info
	GroundEntity* GetTerrain(void);

	// Update
	void Update(double dt = 0.0333f);

	// Constrain the position within the borders
	void Constrain(void);
	// Render
	void Render(void);
};

