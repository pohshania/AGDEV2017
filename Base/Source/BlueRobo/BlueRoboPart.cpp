#include "BlueRoboPart.h"
#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"

CBlueRoboPart::CBlueRoboPart()
	: GenericEntity(NULL)
{
	
}

CBlueRoboPart::~CBlueRoboPart()
{
}

void CBlueRoboPart::Init()
{
	//EntityManager::GetInstance()->AddEntity(this, true);
}

void CBlueRoboPart::Reset()
{
}

void CBlueRoboPart::Update(double dt)
{
}

void CBlueRoboPart::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	if (GetLODStatus() == true)
	{
		if (theDetailLevel != NO_DETAILS)
		{
			RenderHelper::RenderMesh(GetLODMesh());
		}
	}
	modelStack.PopMatrix();
}