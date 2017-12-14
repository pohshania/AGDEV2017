#include "BlueRobo.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

CBlueRobo::CBlueRobo()
	: GenericEntity(NULL)
	, defaultPosition(Vector3(0.0f, 0.0f, 0.0f))
	, defaultTarget(Vector3(0.0f, 0.0f, 0.0f))
	, defaultUp(Vector3(0.0f, 0.0f, 0.0f))
	, target(Vector3(0.0f, 0.0f, 0.0f))
	, up(Vector3(0.0f, 0.0f, 0.0f))
	, maxBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, minBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, m_pTerrain(NULL)
{
}


CBlueRobo::~CBlueRobo()
{
}

void CBlueRobo::Init(void)
{
	// Set the default values
	defaultPosition.Set(0.0f, -10.0f, 0.0f);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set the current values
	position.Set(0.0f, -10.0f, 0.0f);
	target.Set(10.0f, 0.0f, 450.0f);
	up.Set(0.0f, 1.0f, 0.0f);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	// Set speed
	m_dSpeed = 0.05;

	// Initialise the LOD meshes
	InitLOD("BlueRobo", "BlueRobo", "cubeSG");

	// Initialise the Collider
	//this->SetScale(Vector3(2, 2, 2));
	this->SetCollider(true);
	this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));

	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this, true);

}

// Reset this player instance to default
void CBlueRobo::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

// Set position
void CBlueRobo::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void CBlueRobo::SetTarget(const Vector3& target)
{
	this->target = target;
}
// Set Up
void CBlueRobo::SetUp(const Vector3& up)
{
	this->up = up;
}
// Set the boundary for the player info
void CBlueRobo::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}
// Set the terrain for the player info
void CBlueRobo::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}

// Get position
Vector3 CBlueRobo::GetPos(void) const
{
	return position;
}

// Get target
Vector3 CBlueRobo::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 CBlueRobo::GetUp(void) const
{
	return up;
}
// Get the terrain for the player info
GroundEntity* CBlueRobo::GetTerrain(void)
{
	return m_pTerrain;
}

// Update
void CBlueRobo::Update(double dt)
{
	//Vector3 viewVector = (target - position).Normalized();
	//position += viewVector * (float)m_dSpeed * (float)dt;
	////cout << position << "..." << viewVector << endl;

	//// Constrain the position
	//Constrain();

	//// Update the target
	//if (position.z > 400.0f)
	//	target.z = position.z * -1;
	//else if (position.z < -400.0f)
	//	target.z = position.z * -1;
	directionVecX = CPlayerInfo::GetInstance()->GetPos().x - GetPos().x;
	directionVecZ = CPlayerInfo::GetInstance()->GetPos().z - GetPos().z;

	position.x += directionVecX * (float)m_dSpeed * (float)dt;
	position.z += directionVecZ * (float)m_dSpeed * (float)dt;


}

// Constrain the position within the borders
void CBlueRobo::Constrain(void)
{
	// Constrain player within the boundary
	if (position.x > maxBoundary.x - 1.0f)
		position.x = maxBoundary.x - 1.0f;
	if (position.z > maxBoundary.z - 1.0f)
		position.z = maxBoundary.z - 1.0f;
	if (position.x < minBoundary.x + 1.0f)
		position.x = minBoundary.x + 1.0f;
	if (position.z < minBoundary.z + 1.0f)
		position.z = minBoundary.z + 1.0f;

	// if the y position is not equal to terrain height at that position, 
	// then update y position to the terrain height
	if (position.y != m_pTerrain->GetTerrainHeight(position))
		position.y = m_pTerrain->GetTerrainHeight(position);
}

// Render
void CBlueRobo::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (GetLODStatus() == true)
	{
		if (theDetailLevel != NO_DETAILS)
		{
			//cout << theDetailLevel << endl;
			RenderHelper::RenderMesh(GetLODMesh());
		}
	}
	modelStack.PopMatrix();
}