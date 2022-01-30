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
	//// draw target bounding box
	//Util::DrawRect(m_pTarget->getTransform()->position - glm::vec2(m_pTarget->getWidth() * 0.5f, m_pTarget->getHeight()*0.5f), 
	//	m_pTarget->getWidth(), m_pTarget->getHeight());

	//Util::DrawCircle(m_pTarget->getTransform()->position, m_pTarget->getWidth() * 0.5f);

	//// draw spaceship bounding box
	//Util::DrawRect(m_pSpaceShip->getTransform()->position - glm::vec2(m_pSpaceShip->getWidth() * 0.5f, m_pSpaceShip->getHeight() * 0.5f), 
	//	m_pSpaceShip->getWidth(), m_pSpaceShip->getHeight());
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();

	//CollisionManager::AABBCheck(m_pTarget, m_pSpaceShip);
	//CollisionManager::circleAABBCheck(m_pTarget, m_pSpaceShip);
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

	m_pTarget = new Target(); // Instantiating a new Target object - allocating memory on the Heap
	addChild(m_pTarget);

	m_pSpaceShip = new SpaceShip();
	m_pSpaceShip->setCurrentHeading(0.0);
	m_pSpaceShip->setTargetPosition(m_pTarget->getTransform()->position);
	m_pSpaceShip->getRigidBody()->velocity = m_pSpaceShip->getCurrentDirection() * m_pSpaceShip->getMaxSpeed();
	m_pSpaceShip->getRigidBody()->acceleration = m_pSpaceShip->getCurrentDirection() * m_pSpaceShip->getAccelerationRate();
	m_pSpaceShip->setEnabled(false);
	addChild(m_pSpaceShip);
	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Lab 2 Debug Properties", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::Separator();

	// target properties

	static float position[2] = {m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y };
	if(ImGui::SliderFloat2("Target Position", position, 0.0f, 800.0f))
	{
		m_pTarget->getTransform()->position = glm::vec2(position[0], position[1]);
		m_pSpaceShip->setTargetPosition(m_pTarget->getTransform()->position);
	}
	
	ImGui::Separator();

	// spaceship properties
	static bool toggleSeek = m_pSpaceShip->isEnabled();
	if (ImGui::Checkbox("Toggle Seek", &toggleSeek))
	{
		m_pSpaceShip->setEnabled(toggleSeek);
	}

	static float speed = m_pSpaceShip->getMaxSpeed();
	if (ImGui::SliderFloat("Max Speed", &speed, 0.0f, 100.0f))
	{
		m_pSpaceShip->setMaxSpeed(speed);
		m_pSpaceShip->getRigidBody()->velocity = m_pSpaceShip->getCurrentDirection() * m_pSpaceShip->getMaxSpeed();
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

	ImGui::End();
}
