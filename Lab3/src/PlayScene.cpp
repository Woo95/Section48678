#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	drawDisplayList();

	if(m_bDebugView)
	{
		//Util::DrawRect(m_pTarget->getTransform()->position - glm::vec2(m_pTarget->getWidth() * 0.5f, m_pTarget->getHeight() * 0.5f), m_pTarget->getWidth(), m_pTarget->getHeight());
		Util::DrawCircle(m_pTarget->getTransform()->position, m_pTarget->getWidth() * 0.5f);

		
		if (m_pSpaceShip->isEnabled())
		{
			//Util::DrawCircle(m_pSpaceShip->getTransform()->position, Util::max(m_pSpaceShip->getWidth() * 0.5f, m_pSpaceShip->getHeight() * 0.5f));
			Util::DrawRect(m_pSpaceShip->getTransform()->position - glm::vec2(m_pSpaceShip->getWidth() * 0.5f, m_pSpaceShip->getHeight() * 0.5f), m_pSpaceShip->getWidth(), m_pSpaceShip->getHeight());
	
			Util::DrawLine(m_pSpaceShip->getTransform()->position, m_pSpaceShip->getLeftLOSEndPoint(), m_pSpaceShip->getLineColor(0));
			Util::DrawLine(m_pSpaceShip->getTransform()->position, m_pSpaceShip->getMiddleLOSEndPoint(), m_pSpaceShip->getLineColor(1));
			Util::DrawLine(m_pSpaceShip->getTransform()->position, m_pSpaceShip->getRightLOSEndPoint(), m_pSpaceShip->getLineColor(2));
			//Util::DrawLine(m_pSpaceShip->getTransform()->position, m_pSpaceShip->getLeftSideLOSEndPoint(), m_pSpaceShip->getLineColor(3));
			//Util::DrawLine(m_pSpaceShip->getTransform()->position, m_pSpaceShip->getRightSideLOSEndPoint(), m_pSpaceShip->getLineColor(4));
		}

	}
	
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();

	if(m_pSpaceShip->isEnabled())
	{
		//CollisionManager::squaredRadiusCheck(m_pSpaceShip, m_pTarget);
		CollisionManager::circleAABBCheck(m_pTarget, m_pSpaceShip);
		CollisionManager::AABBCheck(m_pObstacle, m_pSpaceShip);
		CollisionManager::rotateAABB(m_pSpaceShip, m_pSpaceShip->getCurrentHeading());
	}
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance().quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance().changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance().changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";
	m_bDebugView = false;


	m_pTarget = new Target(); // instantiating a new Target object - allocating memory on the Heap
	addChild(m_pTarget);

	m_pSpaceShip = new SpaceShip();
	m_pSpaceShip->setCurrentHeading(0.0);
	m_pSpaceShip->setTargetPosition(m_pTarget->getTransform()->position);
	m_pSpaceShip->getRigidBody()->acceleration = m_pSpaceShip->getCurrentDirection() * m_pSpaceShip->getAccelerationRate();
	m_pSpaceShip->setEnabled(false);
	m_pSpaceShip->setWhiskerAngle(45.0f);

	addChild(m_pSpaceShip);

	m_pObstacle = new Obstacle();
	addChild(m_pObstacle);

	// Add two audio load lines 
	//SoundManager::Instance().load("../Assets/audio/mainBGM.mp3", "mainBGM", SOUND_MUSIC);
	//SoundManager::Instance().playMusic("mainBGM", -1, 0);

	SoundManager::Instance().load("../Assets/audio/yay.ogg", "yay", SOUND_SFX);
	//SoundManager::Instance().playSound("yay", 0, -1);
	SoundManager::Instance().load("../Assets/audio/thunder.ogg", "boom", SOUND_SFX);
	//SoundManager::Instance().playSound("boom", 0, -1);


	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Lab 3 Debug Properties", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::Separator();

	static bool toggleDebug = false;
	if (ImGui::Checkbox("Toggle Debug", &toggleDebug))
	{
		m_bDebugView = toggleDebug;
	}
	
	ImGui::Separator();

	// target properties
	
	static float position[2] = { m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y};
	if(ImGui::SliderFloat2("Target Position", position, 0.0f, 800.0f))
	{
		m_pTarget->getTransform()->position = glm::vec2(position[0], position[1]);
		m_pSpaceShip->setTargetPosition(m_pTarget->getTransform()->position);
	}

	static float obstacle[2] = { m_pObstacle->getTransform()->position.x, m_pObstacle->getTransform()->position.y };
	if (ImGui::SliderFloat2("Obstacle Position", obstacle, 0.0f, 800.0f))
	{
		m_pObstacle->getTransform()->position = glm::vec2(obstacle[0], obstacle[1]);
	}

	ImGui::Separator();

	// spaceship properties
	static bool toggleSeek = m_pSpaceShip->isEnabled();
	if(ImGui::Checkbox("Toggle Seek", &toggleSeek))
	{
		m_pSpaceShip->setEnabled(toggleSeek);
	}

	static float speed = m_pSpaceShip->getMaxSpeed();
	if(ImGui::SliderFloat("Max Speed", &speed, 0.0f, 100.0f))
	{
		m_pSpaceShip->setMaxSpeed(speed);
	}

	static float acceleration = m_pSpaceShip->getAccelerationRate();
	if (ImGui::SliderFloat("Acceleration Rate", &acceleration, 0.0f, 50.0f))
	{
		m_pSpaceShip->setAccelerationRate(acceleration);
		m_pSpaceShip->getRigidBody()->acceleration = m_pSpaceShip->getCurrentDirection() * m_pSpaceShip->getAccelerationRate();
	}

	static float turn_rate = m_pSpaceShip->getTurnRate();
	if (ImGui::SliderFloat("Turn Rate", &turn_rate, 0.0f, 20.0f))
	{
		m_pSpaceShip->setTurnRate(turn_rate);
	}

	static float whisker_angle = m_pSpaceShip->getWhiskerAngle();
	if (ImGui::SliderFloat("Whisker Angle", &whisker_angle, 10.0f, 60.0f))
	{
		m_pSpaceShip->updateWhiskers(whisker_angle);
	}

	if(ImGui::Button("Reset"))
	{
		// reset ship's position
		m_pSpaceShip->getTransform()->position = glm::vec2(100.0f, 400.0f);
		// reset target's position
		m_pTarget->getTransform()->position = glm::vec2(500.0f, 100.0f);

		// reset current heading (orientation), velocity and acceleration
		m_pSpaceShip->setCurrentHeading(0.0);
		m_pSpaceShip->getRigidBody()->velocity = glm::vec2(0, 0);
		m_pSpaceShip->getRigidBody()->acceleration = m_pSpaceShip->getCurrentDirection() * m_pSpaceShip->getAccelerationRate();

		m_pSpaceShip->setTargetPosition(m_pTarget->getTransform()->position);
	}
	
	ImGui::End();
}
