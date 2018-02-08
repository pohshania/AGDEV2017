#include "Enemy.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../Waypoint/WaypointManager.h"
#include "MeshBuilder.h"
#include "../Projectile/Projectile.h"
//#include "EnemyChase.h"
//#include "EnemyPatrol.h"

CEnemy::CEnemy()
	: GenericEntity(NULL)
	, defaultPosition(Vector3(0.0f,0.0f,0.0f))
	, defaultTarget(Vector3(0.0f, 0.0f, 0.0f))
	, defaultUp(Vector3(0.0f, 0.0f, 0.0f))
	, target(Vector3(0.0f, 0.0f, 0.0f))
	, up(Vector3(0.0f, 0.0f, 0.0f))
	, maxBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, minBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, m_pTerrain(NULL)
	, m_iWayPointIndex(-1)
{
	listOfWaypoints.clear();
}


CEnemy::~CEnemy()
{
}

void CEnemy::Init(void)
{
	//EnemyStatesBase* ES;
	//ES = new EnemyChase();
	//ES->Enter(*this);
	////delete ES;


	// Enemy start with patrol state
	_currState = E_PATROL;

	reached = false;

	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set up the waypoints
	listOfWaypoints.push_back(0);
	listOfWaypoints.push_back(1);
	listOfWaypoints.push_back(2);

	m_iWayPointIndex = 0;

	// Set the current values
	position.Set(100.0f, 0.0f, 0.0f);
	//target.Set(10.0f, 0.0f, 450.0f);
	CWaypoint* nextWaypoint = GetNextWaypoint();
	if (nextWaypoint)
		target = nextWaypoint->GetPosition();
	else
		target = Vector3(0, 0, 0);
	cout << "Next target: " << target << endl;
	up.Set(0.0f, 1.0f, 0.0f);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	// Set speed
	m_dSpeed = 0.1;

	// Initialise the LOD meshes
	InitLOD("cube", "sphere", "cubeSG");

	// Initialise the Collider
	this->SetCollider(true);
	this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));
	this->SetScale(Vector3(3, 3, 3));

	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this, true);

}

void CEnemy::ChangeState(ENEMY_STATES state)
{
	switch (state)
	{
	case E_PATROL:
	{
		//cout << "PATROLLING NOW!" << endl;
		PatrolState();
		break;
	}
	case E_CHASE:
	{
		//cout << "CHASING NOW!" << endl;
		ChaseState();
		break;
	}
	case E_ATTACK:
	{
		//cout << "ATTACKING NOW!" << endl;
		AttackState();
		break;
	}
	case E_RETURN:
	{
		//cout << "RETURN NOW!" << endl;
		ReturnState();
		break;
	}
	}
}

// green
void CEnemy::PatrolState()
{
	reached = false;

	// patrol -> chase
	if ((GetPos().x - CPlayerInfo::GetInstance()->GetPos().x) < 50.0f)
	{
		_currState = E_CHASE;
	}	
}

// orange
void CEnemy::ChaseState()
{
	reached = false;

	// chase -> attack
	if ((GetPos().x - CPlayerInfo::GetInstance()->GetPos().x) < 25.0f)
	{
		_currState = E_ATTACK;
	}

	// chase -> return
	if ((GetPos().x - CPlayerInfo::GetInstance()->GetPos().x) > 60.0f)
	{
		_currState = E_RETURN;
	}
}

// red
void CEnemy::AttackState()
{
	//// attack -> return
	//if ((GetPos().x - CPlayerInfo::GetInstance()->GetPos().x) > 50.0f)
	//{
	//	_currState = E_RETURN;
	//}

	// attack -> chase
	if ((GetPos().x - CPlayerInfo::GetInstance()->GetPos().x) > 40.0f)
	{
		_currState = E_CHASE;
	}

	
}

// blue
void CEnemy::ReturnState()
{
	if (!reached)
	{
		// return -> chase
		if ((GetPos().x - CPlayerInfo::GetInstance()->GetPos().x) < 40.0f)
		{
			_currState = E_CHASE;
		}
	}
	if (reached == true)
	{
		// return -> patrol
		_currState = E_PATROL;
	}
}

// Reset this player instance to default
void CEnemy::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

// Set position
void CEnemy::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void CEnemy::SetTarget(const Vector3& target)
{
	this->target = target;
}
// Set Up
void CEnemy::SetUp(const Vector3& up)
{
	this->up = up;
}
// Set the boundary for the player info
void CEnemy::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}
// Set the terrain for the player info
void CEnemy::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}

// Get position
Vector3 CEnemy::GetPos(void) const
{
	return position;
}

// Get target
Vector3 CEnemy::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 CEnemy::GetUp(void) const
{
	return up;
}
// Get the terrain for the player info
GroundEntity* CEnemy::GetTerrain(void)
{
	return m_pTerrain;
}

// Get next Waypoint for this CEnemy
CWaypoint* CEnemy::GetNextWaypoint(void)
{
	if ((int)listOfWaypoints.size() > 0)
	{
		m_iWayPointIndex++;
		if (m_iWayPointIndex >= (int)listOfWaypoints.size())
			m_iWayPointIndex = 0;
		return CWaypointManager::GetInstance()->GetWaypoint(listOfWaypoints[m_iWayPointIndex]);
	}
	else
		return NULL;
}

// Update
void CEnemy::Update(double dt)
{
	ChangeState(_currState);
	//cout << _currState;
	//cout << GetPos().x - CPlayerInfo::GetInstance()->GetPos().x << endl;

	//Vector3 viewVector = (target - position).Normalized();
	//position += viewVector * (float)m_dSpeed * (float)dt;
	////cout << position << "..." << viewVector << endl;

	//// Constrain the position
	//Constrain();

	//// Update the target
	///*
	//if (position.z > 400.0f)
	//target.z = position.z * -1;
	//else if (position.z < -400.0f)
	//target.z = position.z * -1;
	//*/

	//if ((target - position).LengthSquared() < 25.0f)
	//{
	//	CWaypoint* nextWaypoint = GetNextWaypoint();
	//	if (nextWaypoint)
	//		target = nextWaypoint->GetPosition();
	//	else
	//		target = Vector3(0, 0, 0);
	//	cout << "Next target: " << target << endl;
	//}

	// controls the state movement
	switch (_currState)
	{
	case E_PATROL:
	{
		Vector3 viewVector = (target - position).Normalized();
		position += viewVector * ((float)m_dSpeed * 100) * (float)dt;

		// Constrain the position
		Constrain();

		if ((target - position).LengthSquared() < 25.0f)
		{
			CWaypoint* nextWaypoint = GetNextWaypoint();
			if (nextWaypoint)
				target = nextWaypoint->GetPosition();
			else
				target = Vector3(0, 0, 0);
			cout << "Next target: " << target << endl;
		}
		break;
	}
	case E_CHASE:
	{
		// follow player
		directionVecX = CPlayerInfo::GetInstance()->GetPos().x - GetPos().x;
		directionVecZ = CPlayerInfo::GetInstance()->GetPos().z - GetPos().z;

		position.x += directionVecX * (float)m_dSpeed * (float)dt;
		position.z += directionVecZ * (float)m_dSpeed * (float)dt;

		break;
	}
	case E_ATTACK:
	{
		// follow player
		directionVecX = CPlayerInfo::GetInstance()->GetPos().x - GetPos().x;
		directionVecZ = CPlayerInfo::GetInstance()->GetPos().z - GetPos().z;

		position.x += directionVecX * (float)m_dSpeed * (float)dt;
		position.z += directionVecZ * (float)m_dSpeed * (float)dt;

		break;
	}
	case E_RETURN:
	{
		CWaypoint* nearestWaypoint = CWaypointManager::GetInstance()->GetNearestWaypoint(position);

		//cout << "Nearest Way Point = " << nearestWaypoint << endl;

		//CWaypoint* nextWaypoint = nearestWaypoint;

		if ((target - position).LengthSquared() < 25.0f)
		{
			CWaypoint* nextWaypoint = nearestWaypoint;
			if (nextWaypoint)
			{
				target = nextWaypoint->GetPosition();
			}
				
			if (target == nextWaypoint->GetPosition())
			{
				reached = true;
			}
		/*	else
				reached = true;*/
		}

		Vector3 viewVector = (target - position).Normalized();
		position += viewVector * ((float)m_dSpeed * 100) * (float)dt;

		// Constrain the position
		Constrain();

		break;
	}
	}
}

// Constrain the position within the borders
void CEnemy::Constrain(void)
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
void CEnemy::Render(void)
{
	// this render function is updated every sec
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	//if (GetLODStatus() == true)
	//{
	//	if (theDetailLevel != NO_DETAILS)
	//	{
	//		//cout << theDetailLevel << endl;
	//		RenderHelper::RenderMesh(GetLODMesh());
	//	}
	//}
	switch (_currState)
	{
	case E_PATROL:
	{
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("EnemyPatrol"));
		break;
	}
	case E_CHASE:
	{
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("EnemyChase"));
		break;
	}
	case E_ATTACK:
	{
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("EnemyAttack"));
		break;
	}
	case E_RETURN:
	{
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("EnemyReturn"));
		break;
	}
	}

	modelStack.PopMatrix();

	//cout << "HEYAAAAAAAAAAAA";
}