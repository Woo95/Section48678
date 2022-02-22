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
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();
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

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_R))
	{
		TheGame::Instance().changeSceneState(START_SCENE);
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";
	
	// Plane Sprite
	m_spaceShip = new SpaceShip();
	addChild(m_spaceShip);

	// Target Sprite
	m_target = new Target();
	addChild(m_target);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press R to restart", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(103.0f, 10.0f);

	m_pInstructionsLabel1 = new Label("Press F to calculate shortest route", "Consolas");
	m_pInstructionsLabel1->getTransform()->position = glm::vec2(195.0f, 35.0f);

	m_pInstructionsLabel2 = new Label("Press M to move along shortest route", "Consolas");
	m_pInstructionsLabel2->getTransform()->position = glm::vec2(200.0f, 60.0f);

	addChild(m_pInstructionsLabel);
	addChild(m_pInstructionsLabel1);
	addChild(m_pInstructionsLabel2);

	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Debug Menu", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	if(ImGui::Button("My Button"))
	{
		std::cout << "My Button Pressed" << std::endl;
	}

	ImGui::Separator();

	static float start_position[2] = { m_spaceShip->getTransform()->position.x, m_spaceShip->getTransform()->position.y };
	if(ImGui::SliderFloat2("Start Position", start_position, 0.0f, 500.0f))
	{
		m_spaceShip->getTransform()->position = glm::vec2(start_position[0], start_position[1]);
	}

	static float goal_position[2] = { m_target->getTransform()->position.x, m_target->getTransform()->position.y };
	if (ImGui::SliderFloat2("Goal Position", goal_position, 0.0f, 500.0f))
	{
		m_target->getTransform()->position = glm::vec2(goal_position[0], goal_position[1]);
	}
	
	ImGui::End();
}
