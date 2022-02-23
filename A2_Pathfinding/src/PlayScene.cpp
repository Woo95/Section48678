#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"
#include "Config.h"

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


	if (EventManager::Instance().keyPressed(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance().quit();
	}

	if (EventManager::Instance().keyPressed(SDL_SCANCODE_R))
	{
		TheGame::Instance().changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().keyPressed(SDL_SCANCODE_H))
	{
		if (!m_isGridEnabled)
		{
			m_isGridEnabled = true;
			m_setGridEnabled(m_isGridEnabled);
		}
		
		else 
		{
			m_isGridEnabled = false;
			m_setGridEnabled(m_isGridEnabled);
		}
	}

	// To use the variables for the left and right click events 
	auto x = EventManager::Instance().getMousePosition().x / 40;
	auto y = EventManager::Instance().getMousePosition().y / 40;
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	// pressing left click while grid is enabled let you move the spaceship around the grid.
	if (m_isGridEnabled == true && EventManager::Instance().getMouseButton(0) && m_target->getTransform()->position != m_getTile(x, y)->getTransform()->position + offset)
	{
		m_getTile(m_spaceShip->getGridPosition().x, m_spaceShip->getGridPosition().y)->setTileStatus(UNVISITED);
		m_spaceShip->setGridPosition(x, y);
		m_spaceShip->getTransform()->position = m_getTile(x, y)->getTransform()->position + offset;
		m_getTile(x, y)->setTileStatus(START);
	}
	// pressing right click while grid is enabled let you move the target around the grid.
	if (m_isGridEnabled == true && EventManager::Instance().getMouseButton(2) && m_spaceShip->getTransform()->position != m_getTile(x, y)->getTransform()->position + offset)
	{
		m_getTile(m_target->getGridPosition().x, m_target->getGridPosition().y)->setTileStatus(UNVISITED);
		m_target->setGridPosition(x, y);
		m_target->getTransform()->position = m_getTile(x, y)->getTransform()->position + offset;
		m_getTile(x, y)->setTileStatus(GOAL);
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Set SFX and Music
	SoundManager::Instance().load("../Assets/audio/mainBGM.mp3", "main", SOUND_MUSIC);
	SoundManager::Instance().playMusic("main", -1, 0);
	SoundManager::Instance().setMusicVolume(5);

	m_buildGrid();

	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	m_currentHeuristic = MANHATTAN;
	
	m_target = new Target(); // instantiating a new Target object - allocating memory on the Heap
	m_target->getTransform()->position = m_getTile(15, 11)->getTransform()->position + offset;
	m_target->setGridPosition(15, 11);
	m_getTile(15, 11)->setTileStatus(GOAL);
	addChild(m_target);

	m_spaceShip = new SpaceShip();
	m_spaceShip->getTransform()->position = m_getTile(1, 3)->getTransform()->position + offset;
	m_spaceShip->setGridPosition(1, 3);
	m_getTile(1, 3)->setTileStatus(START);
	addChild(m_spaceShip);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press R to restart", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(103.0f, 10.0f);

	m_pInstructionsLabel1 = new Label("Press F to calculate shortest route", "Consolas");
	m_pInstructionsLabel1->getTransform()->position = glm::vec2(195.0f, 35.0f);

	m_pInstructionsLabel2 = new Label("Press M to move along shortest route", "Consolas");
	m_pInstructionsLabel2->getTransform()->position = glm::vec2(200.0f, 60.0f);

	m_pInstructionsLabel3 = new Label("Press H to toggle debug view", "Consolas");
	m_pInstructionsLabel3->getTransform()->position = glm::vec2(156.0f, 85.0f);

	addChild(m_pInstructionsLabel);
	addChild(m_pInstructionsLabel1);
	addChild(m_pInstructionsLabel2);
	addChild(m_pInstructionsLabel3);

	m_computeTileCost();

	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::m_buildGrid()
{
	const auto tile_size = Config::TILE_SIZE;

	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			Tile* tile = new Tile();
			tile->getTransform()->position = glm::vec2(col * tile_size, row * tile_size); // world position
			tile->setGridPosition(col, row);
			tile->setParent(this);
			tile->addLabels();
			addChild(tile);
			tile->setEnabled(false);
			m_pGrid.push_back(tile);
		}
	}

	// setup neightbour refernece 
	// Tiles = nodes in our graph
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			Tile* tile = m_getTile(col, row);

			// top
			if (row == 0)
			{
				tile->setNeighbourTile(TOP_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(TOP_TILE, m_getTile(col, row - 1));
			}

			// right
			if (col == Config::COL_NUM - 1)
			{
				tile->setNeighbourTile(RIGHT_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(RIGHT_TILE, m_getTile(col + 1, row));
			}

			// bottom
			if (row == Config::ROW_NUM - 1)
			{
				tile->setNeighbourTile(BOTTOM_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(BOTTOM_TILE, m_getTile(col, row + 1));
			}

			// left
			if (col == 0)
			{
				tile->setNeighbourTile(LEFT_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(LEFT_TILE, m_getTile(col - 1, row));
			}
		}
	}
}

bool PlayScene::m_getGridEnabled() const
{
	return m_isGridEnabled;
}

void PlayScene::m_setGridEnabled(const bool state)
{
	m_isGridEnabled = state;

	for (auto tile : m_pGrid)
	{
		tile->setEnabled(m_isGridEnabled);
		tile->setLabelsEnabled(m_isGridEnabled);
	}
}

void PlayScene::m_computeTileCost()
{
	float distance = 0.0f;
	float dx = 0.0f;
	float dy = 0.0f;

	// for each tile in the grid, loop
	for (auto tile : m_pGrid)
	{
		// compute the distance from each tile to the goal tile
		// distance (f) = tile cost (g) + heuristic estimate (h)
		switch (m_currentHeuristic)
		{
		case MANHATTAN:
			dx = abs(tile->getGridPosition().x - m_target->getGridPosition().x);
			dy = abs(tile->getGridPosition().y - m_target->getGridPosition().y);

			distance = dx + dy;
			break;
		case EUCLIDEAN:

			// computes euclidean distance ("as the crow flies") for each tile 
			distance + Util::distance(tile->getGridPosition(), m_target->getGridPosition());
			break;
		}

		tile->setTileCost(distance);

	}
}

Tile* PlayScene::m_getTile(const int col, const int row)
{
	return m_pGrid[(row * Config::COL_NUM) + col];
}

Tile* PlayScene::m_getTile(glm::vec2 grid_position)
{
	const auto col = grid_position.x;
	const auto row = grid_position.y;

	return m_getTile(col, row);
}

void PlayScene::GUI_Function()
{
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Assignment 2 Debug Properties", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::Separator();

	// grid properties

	static bool toggleGrid = false;
	if (ImGui::Checkbox("Toggle Grid", &toggleGrid))
	{
		m_isGridEnabled = toggleGrid;
		m_setGridEnabled(m_isGridEnabled);
	}

	ImGui::Separator();

	// heuristic selection

	static int radio = m_currentHeuristic;

	ImGui::Text("Heuristic Type");
	ImGui::RadioButton("Manhatten", &radio, MANHATTAN);
	ImGui::SameLine();
	ImGui::RadioButton("Euclidean", &radio, EUCLIDEAN);

	// check if current heuristic is not the same as current selection
	if (m_currentHeuristic != radio)
	{
		m_currentHeuristic = static_cast<Heuristic>(radio);
		m_computeTileCost();
	}

	ImGui::Separator();

	// spaceship properties


	static int start_position[2] = { m_spaceShip->getGridPosition().x, m_spaceShip->getGridPosition().y };
	if (ImGui::SliderInt2("Start Position", start_position, 0, Config::COL_NUM - 1))
	{
		// constrain object within max row
		if (start_position[1] > Config::ROW_NUM - 1)
		{
			start_position[1] = Config::ROW_NUM - 1;
		}

		m_getTile(m_spaceShip->getGridPosition())->setTileStatus(UNVISITED);
		m_spaceShip->getTransform()->position = m_getTile(start_position[0], start_position[1])->getTransform()->position + offset;
		m_spaceShip->setGridPosition(start_position[0], start_position[1]);
		m_getTile(m_spaceShip->getGridPosition())->setTileStatus(START);
	}

	// target properties

	static int goal_position[2] = { m_target->getGridPosition().x, m_target->getGridPosition().y };
	if (ImGui::SliderInt2("Goal Position", goal_position, 0, Config::COL_NUM - 1))
	{
		// constrain object within max row
		if (goal_position[1] > Config::ROW_NUM - 1)
		{
			goal_position[1] = Config::ROW_NUM - 1;
		}

		m_getTile(m_target->getGridPosition())->setTileStatus(UNVISITED);
		m_target->getTransform()->position = m_getTile(goal_position[0], goal_position[1])->getTransform()->position + offset;
		m_target->setGridPosition(goal_position[0], goal_position[1]);
		m_getTile(m_target->getGridPosition())->setTileStatus(GOAL);
		m_computeTileCost();
	}
	
	ImGui::End();
}
