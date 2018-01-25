#include "SceneText.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "SkyBox/SkyBoxEntity.h"
#include "SceneGraph\SceneGraph.h"
#include "SpatialPartition\SpatialPartition.h"
#include "BlueRobo\BlueRoboSingle.h"
#include "RedRobo\RedRoboSingle.h"

#include <iostream>
using namespace std;

SceneText* SceneText::sInstance = new SceneText(SceneManager::GetInstance());

SceneText::SceneText()
{
}

SceneText::SceneText(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("Start", this);
}

SceneText::~SceneText()
{
	CSceneGraph::GetInstance()->Destroy();
}

void SceneText::Init()
{
	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");
	
	// Tell the shader program to store these uniform locations
	currProg->AddUniform("MVP");
	currProg->AddUniform("MV");
	currProg->AddUniform("MV_inverse_transpose");
	currProg->AddUniform("material.kAmbient");
	currProg->AddUniform("material.kDiffuse");
	currProg->AddUniform("material.kSpecular");
	currProg->AddUniform("material.kShininess");
	currProg->AddUniform("lightEnabled");
	currProg->AddUniform("numLights");
	currProg->AddUniform("lights[0].type");
	currProg->AddUniform("lights[0].position_cameraspace");
	currProg->AddUniform("lights[0].color");
	currProg->AddUniform("lights[0].power");
	currProg->AddUniform("lights[0].kC");
	currProg->AddUniform("lights[0].kL");
	currProg->AddUniform("lights[0].kQ");
	currProg->AddUniform("lights[0].spotDirection");
	currProg->AddUniform("lights[0].cosCutoff");
	currProg->AddUniform("lights[0].cosInner");
	currProg->AddUniform("lights[0].exponent");
	currProg->AddUniform("lights[1].type");
	currProg->AddUniform("lights[1].position_cameraspace");
	currProg->AddUniform("lights[1].color");
	currProg->AddUniform("lights[1].power");
	currProg->AddUniform("lights[1].kC");
	currProg->AddUniform("lights[1].kL");
	currProg->AddUniform("lights[1].kQ");
	currProg->AddUniform("lights[1].spotDirection");
	currProg->AddUniform("lights[1].cosCutoff");
	currProg->AddUniform("lights[1].cosInner");
	currProg->AddUniform("lights[1].exponent");
	currProg->AddUniform("colorTextureEnabled");
	currProg->AddUniform("colorTexture");
	currProg->AddUniform("textEnabled");
	currProg->AddUniform("textColor");
	
	// Tell the graphics manager to use the shader we just loaded
	GraphicsManager::GetInstance()->SetActiveShader("default");

	lights[0] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	lights[0]->position.Set(0, 20, 0);
	lights[0]->color.Set(1, 1, 1);
	lights[0]->power = 1;
	lights[0]->kC = 1.f;
	lights[0]->kL = 0.01f;
	lights[0]->kQ = 0.001f;
	lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	lights[0]->exponent = 3.f;
	lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	lights[0]->name = "lights[0]";

	lights[1] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	lights[1]->type = Light::LIGHT_DIRECTIONAL;
	lights[1]->position.Set(1, 1, 0);
	lights[1]->color.Set(1, 1, 0.5f);
	lights[1]->power = 0.4f;
	lights[1]->name = "lights[1]";

	currProg->UpdateInt("numLights", 1);
	currProg->UpdateInt("textEnabled", 0);
	
	// Create the playerinfo instance, which manages all information about the player
	playerInfo = CPlayerInfo::GetInstance();
	playerInfo->Init();

	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.Init(playerInfo->GetPos(), playerInfo->GetTarget(), playerInfo->GetUp());
	playerInfo->AttachCamera(&camera);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
	MeshBuilder::GetInstance()->GenerateOBJ("Chair", "OBJ//chair.obj");
	MeshBuilder::GetInstance()->GetMesh("Chair")->textureID = LoadTGA("Image//chair.tga");
	MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 0.5f);
	MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);

	// Ground
	MeshBuilder::GetInstance()->GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GRASS_DARKGREEN")->textureID = LoadTGA("Image//SkyBox//skybox_bottom.tga");
	MeshBuilder::GetInstance()->GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN")->textureID = LoadTGA("Image//SkyBox//skybox_bottom.tga");
	MeshBuilder::GetInstance()->GenerateCube("cubeSG", Color(1.0f, 0.64f, 0.0f), 1.0f);

	// SkyBox
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//SkyBox//skybox_front.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//SkyBox//skybox_back.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//SkyBox//skybox_left.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//SkyBox//skybox_right.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//SkyBox//skybox_top.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//SkyBox//skybox_bottom.tga");

	// Laser
	MeshBuilder::GetInstance()->GenerateRay("laser", 10.0f);
	MeshBuilder::GetInstance()->GenerateQuad("GRIDMESH", Color(1, 1, 1), 10.f);

	// Enemy Base
	MeshBuilder::GetInstance()->GenerateOBJ("EnemyBase", "OBJ//Enemy//EnemyBase.obj");
	MeshBuilder::GetInstance()->GetMesh("EnemyBase")->textureID = LoadTGA("Image//Enemy//EnemyBase.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("EnemyBasePart1", "OBJ//Enemy//EnemyBasePart1.obj");
	MeshBuilder::GetInstance()->GetMesh("EnemyBasePart1")->textureID = LoadTGA("Image//Enemy//EnemyBase.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("EnemyBasePart2", "OBJ//Enemy//EnemyBasePart2.obj");
	MeshBuilder::GetInstance()->GetMesh("EnemyBasePart2")->textureID = LoadTGA("Image//Enemy//EnemyBase.tga");

	// Blue Robo
	MeshBuilder::GetInstance()->GenerateOBJ("BlueRobo", "OBJ//Enemy//BlueRobo.obj");
	MeshBuilder::GetInstance()->GetMesh("BlueRobo")->textureID = LoadTGA("Image//Enemy//BlueRobo.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("BlueRoboHead", "OBJ//Enemy//BlueRoboHead.obj");
	MeshBuilder::GetInstance()->GetMesh("BlueRoboHead")->textureID = LoadTGA("Image//Enemy//BlueRobo.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("BlueRoboBody", "OBJ//Enemy//BlueRoboBody.obj");
	MeshBuilder::GetInstance()->GetMesh("BlueRoboBody")->textureID = LoadTGA("Image//Enemy//BlueRobo.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("BlueRoboBottom", "OBJ//Enemy//BlueRoboBottom.obj");
	MeshBuilder::GetInstance()->GetMesh("BlueRoboBottom")->textureID = LoadTGA("Image//Enemy//BlueRobo.tga");

	// Red Robo
	MeshBuilder::GetInstance()->GenerateOBJ("RedRoboHead", "OBJ//Enemy//RedRoboHead.obj");
	MeshBuilder::GetInstance()->GetMesh("RedRoboHead")->textureID = LoadTGA("Image//Enemy//RedRobo.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("RedRoboBody", "OBJ//Enemy//RedRoboBody.obj");
	MeshBuilder::GetInstance()->GetMesh("RedRoboBody")->textureID = LoadTGA("Image//Enemy//RedRobo.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("RedRoboBottom", "OBJ//Enemy//RedRoboBottom.obj");
	MeshBuilder::GetInstance()->GetMesh("RedRoboBottom")->textureID = LoadTGA("Image//Enemy//RedRobo.tga");

	// Bullet
	MeshBuilder::GetInstance()->GenerateOBJ("Bullet", "OBJ//bullet.obj");
	MeshBuilder::GetInstance()->GetMesh("Bullet")->textureID = LoadTGA("Image//bullet.tga");

	// Spaceship
	MeshBuilder::GetInstance()->GenerateOBJ("Spaceship", "OBJ//spaceship.obj");
	MeshBuilder::GetInstance()->GetMesh("Spaceship")->textureID = LoadTGA("Image//spaceship.tga");

	// Set up the Spatial Partition and pass it to the EntityManager to manage
	CSpatialPartition::GetInstance()->Init(100, 100, 10, 10);
	CSpatialPartition::GetInstance()->SetMesh("GRIDMESH");

	// LOD
	CSpatialPartition::GetInstance()->SetCamera(&camera);
	CSpatialPartition::GetInstance()->SetLevelOfDetails(40000.0f, 160000.0f);

	EntityManager::GetInstance()->SetSpatialPartition(CSpatialPartition::GetInstance());

	MeshBuilder::GetInstance()->GenerateQuad("PlayerHighlight", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("PlayerHighlight")->textureID = LoadTGA("Image//PlayerHighlight.tga");

	MeshBuilder::GetInstance()->GenerateQuad("BlueHighlight", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("BlueHighlight")->textureID = LoadTGA("Image//BlueHighlight.tga");

	MeshBuilder::GetInstance()->GenerateQuad("RedHighlight", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("RedHighlight")->textureID = LoadTGA("Image//RedHighlight.tga");

	// Create entities into the scene
	//Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	//Create::Entity("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z)); // Lightball

	//GenericEntity* aCube = Create::Entity("cube", Vector3(-20.0f, 0.0f, -20.0f));
	//aCube->SetCollider(true);
	//aCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	//aCube->InitLOD("cube", "sphere", "cubeSG");

	//// Add the pointer to this new entity to the Scene Graph
	//CSceneNode* theNode = CSceneGraph::GetInstance()->AddNode(aCube);
	//if (theNode == NULL)
	//{
	//	cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	//}

	//GenericEntity* anotherCube = Create::Entity("cube", Vector3(-20.0f, 5.0f, -20.0f));
	//anotherCube->SetCollider(true);
	//anotherCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	//CSceneNode* anotherNode = theNode->AddChild(anotherCube);
	//if (anotherNode == NULL)
	//{
	//	cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	//}



	// BLUE ROBOBOBOBOBOBOBOBOBOBOBOBOBOBOBOBOBOBOBOBO !!@$@#%@#%
	//GenericEntity* anotherCube2 = Create::Entity("BlueRoboHead", Vector3(0.0f, 4.0f, 50.0f));
	//anotherCube2->SetCollider(true);
	//anotherCube2->SetAABB(Vector3(1.0f, 1.0f, 1.0f), Vector3(-1.0f, -1.0f, -1.0f));
	//GenericEntity* anotherCube3 = Create::Entity("BlueRoboBody", Vector3(0.0f, 0.0f, 50.0f));
	//anotherCube3->SetCollider(true);
	//anotherCube3->SetAABB(Vector3(1.0f, 1.0f, 1.0f), Vector3(-1.0f, -1.0f, -1.0f));
	//GenericEntity* anotherCube4 = Create::Entity("BlueRoboLeftHand", Vector3(-2.35f, 0.0f, 50.0f));
	//anotherCube4->SetCollider(true);
	//anotherCube4->SetAABB(Vector3(1.0f, 1.0f, 1.0f), Vector3(-1.0f, -1.0f, -1.0f));
	//GenericEntity* anotherCube5 = Create::Entity("BlueRoboRightHand", Vector3(2.35f, 0.0f, 50.0f));
	//anotherCube5->SetCollider(true);
	//anotherCube5->SetAABB(Vector3(1.0f, 1.0f, 1.0f), Vector3(-1.0f, -1.0f, -1.0f));

	// Transform stuff
	//GenericEntity* baseCube = Create::Asset("cube", Vector3(0.0f, 0.0f, 0.0f));
	//CSceneNode* baseNode = CSceneGraph::GetInstance()->AddNode(baseCube);

	//CUpdateTransformation* baseMtx = new CUpdateTransformation();
	//baseMtx->ApplyUpdate(1.0f, 0.0f, 0.0f, 1.0f);
	//baseMtx->SetSteps(-60, 60);
	//baseNode->SetUpdateTransformation(baseMtx);

	//GenericEntity* childCube = Create::Asset("cubeSG", Vector3(0.0f, 0.0f, 0.0f));
	//CSceneNode* childNode = baseNode->AddChild(childCube);
	//childNode->ApplyTranslate(0.0f, 1.0f, 0.0f);

	//GenericEntity* grandchildCube = Create::Asset("cubeSG", Vector3(0.0f, 0.0f, 0.0f));
	//CSceneNode* grandchildNode = childNode->AddChild(grandchildCube);
	//grandchildNode->ApplyTranslate(0.0f, 0.0f, 1.0f);
	//CUpdateTransformation* aRotateMtx = new CUpdateTransformation();
	//aRotateMtx->ApplyUpdate(1.0f, 0.0f, 0.0f, 1.0f);
	//aRotateMtx->SetSteps(-120, 60);
	//grandchildNode->SetUpdateTransformation(aRotateMtx);

	// Creating enemy instances
	//theEnemy = new CEnemy();
	//theEnemy->Init();

	blueRobo1 = new CBlueRobo();
	blueRobo1->Init(40, -2.4, -65);
	BlueRoboSingleton::GetInstance()->BlueRobos.push_back(blueRobo1);

	redRobo1 = new CRedRobo();
	redRobo1->Init(-40, -2.5, -65);
	RedRoboSingleton::GetInstance()->RedRobos.push_back(redRobo1);


	// Enemy Base
	GenericEntity* EnemyBase = Create::Asset("EnemyBase", Vector3(0.0f, 44.5f, -300.0f));
	EnemyBase->SetScale(Vector3(3, 3, 3));
	EnemyBase->SetCollider(true);
	EnemyBase->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	EnemyBase->InitLOD("EnemyBase", "EnemyBase", "cubeSG");
	CSceneNode* EnemyBaseNode = CSceneGraph::GetInstance()->AddNode(EnemyBase) ;

	GenericEntity* EnemyBasePart1 = Create::Asset("EnemyBasePart1", Vector3(0.0f, 44.5f, -300.0f));
	EnemyBasePart1->SetScale(Vector3(3, 3, 3));
	EnemyBasePart1->SetCollider(true);
	EnemyBasePart1->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	EnemyBasePart1->InitLOD("EnemyBasePart1", "EnemyBasePart1", "cubeSG");
	CSceneNode* EnemyBasePart1Node = EnemyBaseNode->AddChild(EnemyBasePart1);

	CUpdateTransformation* basePart1 = new CUpdateTransformation();
	basePart1->ApplyUpdate(0.0f, 0.1f, 0.0f);
	basePart1->SetSteps(-40, 40);
	EnemyBasePart1Node->SetUpdateTransformation(basePart1);

	GenericEntity* EnemyBasePart2 = Create::Asset("EnemyBasePart2", Vector3(0.0f, 44.5f, -300.0f));
	EnemyBasePart2->SetScale(Vector3(3, 3, 3));
	EnemyBasePart2->SetCollider(true);
	EnemyBasePart2->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	EnemyBasePart2->InitLOD("EnemyBasePart2", "EnemyBasePart2", "cubeSG");
	CSceneNode* EnemyBasePart2Node = EnemyBaseNode->AddChild(EnemyBasePart2);

	CUpdateTransformation* basePart2 = new CUpdateTransformation();
	basePart2->ApplyUpdate(0.0f, 0.1f, 0.0f);
	basePart2->SetSteps(-40, 40);
	EnemyBasePart2Node->SetUpdateTransformation(basePart2);

	// Spaceship
	GenericEntity* Spaceship = Create::Entity("Spaceship", Vector3(50.0f, 20.0f, 0.0f));
	Spaceship->SetScale(Vector3(6, 6, 6));
	Spaceship->SetCollider(true);
	Spaceship->SetAABB(Vector3(6.f, 6.f, 6.f), Vector3(-6.f, -6.f, -6.f));
	Spaceship->InitLOD("Spaceship", "sphere", "cube");

	// Create ground
	groundEntity = Create::Ground("GRASS_DARKGREEN", "GEO_GRASS_LIGHTGREEN");

    // Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));

	// Create cross hair
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(20.0f, 20.0f, 20.0f));

	// Create skybox
	SkyBoxEntity* theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
											 "SKYBOX_LEFT", "SKYBOX_RIGHT",
											 "SKYBOX_TOP", "SKYBOX_BOTTOM");

	// Customise the ground entity
	groundEntity->SetPosition(Vector3(0, -10, 0));
	groundEntity->SetScale(Vector3(80.0f, 80.0f, 80.0f));
	groundEntity->SetGrids(Vector3(10.0f, 1.0f, 10.0f));
	playerInfo->SetTerrain(groundEntity);
	//theEnemy->SetTerrain(groundEntity);
	//blueRobo1->SetTerrain(groundEntity);

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 3; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f,1.0f,0.0f));
	}
	textObj[0]->SetText("HELLO WORLD");
}

void SceneText::Update(double dt)
{
	// Update our entities
	EntityManager::GetInstance()->Update(dt);

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	if(KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	if(KeyboardController::GetInstance()->IsKeyDown('5'))
	{
		lights[0]->type = Light::LIGHT_POINT;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('6'))
	{
		lights[0]->type = Light::LIGHT_DIRECTIONAL;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('7'))
	{
		lights[0]->type = Light::LIGHT_SPOT;
	}

	if(KeyboardController::GetInstance()->IsKeyDown('I'))
		lights[0]->position.z -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('K'))
		lights[0]->position.z += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('J'))
		lights[0]->position.x -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('L'))
		lights[0]->position.x += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('O'))
		lights[0]->position.y -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('P'))
		lights[0]->position.y += (float)(10.f * dt);

	if (KeyboardController::GetInstance()->IsKeyReleased('M'))
	{
		CSceneNode* theNode = CSceneGraph::GetInstance()->GetNode(1);
		Vector3 pos = theNode->GetEntity()->GetPosition();
		theNode->GetEntity()->SetPosition(Vector3(pos.x + 50.0f, pos.y, pos.z + 50.0f));
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('N'))
	{
		CSpatialPartition::GetInstance()->PrintSelf();
	}

	// if the left mouse button was released
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		cout << "Left Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{
		cout << "Right Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
	{
		cout << "Middle Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
	}
	// <THERE>

	// Update the player position and other details based on keyboard and mouse inputs
	playerInfo->Update(dt);

	//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)

	GraphicsManager::GetInstance()->UpdateLights(dt);

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	std::ostringstream ss;
	ss.precision(5);
	float fps = (float)(1.f / dt);
	ss << "FPS: " << fps;
	textObj[1]->SetText(ss.str());

	std::ostringstream ss1;
	ss1.precision(4);
	ss1 << "Player:" << playerInfo->GetPos();
	textObj[2]->SetText(ss1.str());
	
	//cout << blueRobo1->GetPosition();
}

void SceneText::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RenderUI();
}

void SceneText::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	playerInfo->DetachCamera();

	if (playerInfo->DropInstance() == false)
	{
#if _DEBUGMODE==1
		cout << "Unable to drop PlayerInfo class" << endl;
#endif
	}

	// Delete the lights
	delete lights[0];
	delete lights[1];
}
