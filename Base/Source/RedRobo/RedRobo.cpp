#include "RedRobo.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

CRedRobo::CRedRobo()
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


CRedRobo::~CRedRobo()
{
}

void CRedRobo::Init(float x, float y, float z)
{
	// Set speed
	m_dSpeed = 0.05f;
	// Set the current values
	position.Set(x, y, z);

	// Initialise the Collider
	this->SetScale(Vector3(2.1, 2.1, 2.1));
	this->SetCollider(true);
	this->SetAABB(Vector3(2, 2, 2), Vector3(-1, -1, -1));
	// Initialise the LOD meshes
	InitLOD("RedRoboBody", "RedRoboBody", "sphere");
	// Add the pointer to this new entity to the Scene Graph
	CSceneNode* bodyNode = CSceneGraph::GetInstance()->AddNode(this);

	//GenericEntity* BlueRoboHead = Create::Entity("BlueRoboHead", Vector3(x, y + 8, z));
	//BlueRoboHead->SetScale(Vector3(2, 2, 2));
	//BlueRoboHead->SetCollider(true);
	//BlueRoboHead->SetAABB(Vector3(1.0f, 1.0f, 1.0f), Vector3(-1.0f, -1.0f, -1.0f));
	//BlueRoboHead->InitLOD("BlueRoboHead", "BlueRoboHead", "sphere");
	//CSceneNode* headNode = bodyNode->AddChild(BlueRoboHead);

	//GenericEntity* BlueRoboLeftHand = Create::Entity("BlueRoboLeftHand", Vector3(x - 4.5f, y, z));
	//BlueRoboLeftHand->SetScale(Vector3(2, 2, 2));
	//BlueRoboLeftHand->SetCollider(true);
	//BlueRoboLeftHand->SetAABB(Vector3(1.0f, 1.0f, 1.0f), Vector3(-1.0f, -1.0f, -1.0f));
	//BlueRoboLeftHand->InitLOD("BlueRoboLeftHand", "BlueRoboLeftHand", "sphere");
	//CSceneNode* leftHandNode = bodyNode->AddChild(BlueRoboLeftHand);

	//GenericEntity* BlueRoboRightHand = Create::Entity("BlueRoboRightHand", Vector3(x + 4.5f, y, z));
	//BlueRoboRightHand->SetScale(Vector3(2, 2, 2));
	//BlueRoboRightHand->SetCollider(true);
	//BlueRoboRightHand->SetAABB(Vector3(1.0f, 1.0f, 1.0f), Vector3(-1.0f, -1.0f, -1.0f));
	//BlueRoboRightHand->InitLOD("BlueRoboRightHand", "BlueRoboRightHand", "sphere");
	//CSceneNode* rightHandNode = bodyNode->AddChild(BlueRoboRightHand);

	redRoboHead = new  CRedRoboHead();
	redRoboHead->Init(x, y + 6.5, z);
	CSceneNode* headNode = bodyNode->AddChild(redRoboHead);

	redRoboBottom = new CRedRoboBottom();
	redRoboBottom->Init(x, y - 5.5f, z);
	CSceneNode* bottomNode = bodyNode->AddChild(redRoboBottom);

	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this, true);

}

// Reset this player instance to default
void CRedRobo::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

// Set position
void CRedRobo::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void CRedRobo::SetTarget(const Vector3& target)
{
	this->target = target;
}
// Set Up
void CRedRobo::SetUp(const Vector3& up)
{
	this->up = up;
}
// Set the boundary for the player info
void CRedRobo::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}
// Set the terrain for the player info
void CRedRobo::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}

// Get position
Vector3 CRedRobo::GetPos(void) const
{
	return position;
}

// Get target
Vector3 CRedRobo::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 CRedRobo::GetUp(void) const
{
	return up;
}
// Get the terrain for the player info
GroundEntity* CRedRobo::GetTerrain(void)
{
	return m_pTerrain;
}

// Update
void CRedRobo::Update(double dt)
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


	// follow player
	directionVecX = CPlayerInfo::GetInstance()->GetPos().x - GetPos().x;
	directionVecZ = CPlayerInfo::GetInstance()->GetPos().z - GetPos().z;


	position.x += directionVecX * (float)m_dSpeed * (float)dt;
	position.z += directionVecZ * (float)m_dSpeed * (float)dt;


}

// Constrain the position within the borders
void CRedRobo::Constrain(void)
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
void CRedRobo::Render(void)
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