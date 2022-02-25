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
	if (m_shipIsMoving)
	{
		m_moveShip();
	}
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
			m_resetPathfinding();
		}
		
		else 
		{
			m_isGridEnabled = false;
			m_setGridEnabled(m_isGridEnabled);
			m_resetPathfinding();
		}
	}

	// pressing left click while grid is enabled let you move the spaceship around the grid.
	if (m_isGridEnabled == true && EventManager::Instance().getMouseButton(0))
	{
		// To use the variables for the left and right click events 
		auto x = (int)EventManager::Instance().getMousePosition().x / 40;
		auto y = (int)EventManager::Instance().getMousePosition().y / 40;
		auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
		
		m_getTile(m_pTarget->getGridPosition())->setTileStatus(GOAL);
		if (m_getTile(x, y)->getTileStatus() == UNVISITED)
		{
			m_getTile(m_pSpaceShip->getGridPosition())->setTileStatus(UNVISITED);
			m_pSpaceShip->setGridPosition(x, y);
			m_pSpaceShip->getTransform()->position = m_getTile(x, y)->getTransform()->position + offset;
			m_getTile(x, y)->setTileStatus(START);
		}
	}
	// pressing right click while grid is enabled let you move the target around the grid.
	if (m_isGridEnabled == true && EventManager::Instance().getMouseButton(2))
	{
		// To use the variables for the left and right click events 
		auto x = (int)EventManager::Instance().getMousePosition().x / 40;
		auto y = (int)EventManager::Instance().getMousePosition().y / 40;
		auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
		if (m_getTile(x, y)->getTileStatus() == UNVISITED)
		{
			m_getTile(m_pTarget->getGridPosition())->setTileStatus(UNVISITED);
			m_pTarget->setGridPosition(x, y);
			m_pTarget->getTransform()->position = m_getTile(x, y)->getTransform()->position + offset;
			m_getTile(x, y)->setTileStatus(GOAL);
		}
		m_computeTileCost();
	}

	// Press F to reset and then find shortPath
	if (EventManager::Instance().keyPressed(SDL_SCANCODE_F))
	{
		if (m_isGridEnabled == true)
		{
			m_resetPathfinding();
			m_findShortestPath();
			m_setGridEnabled(false);
			m_displayPathList();
			m_playMoveSound = !m_playMoveSound;
		}
	}
	// Press M to move
	if (EventManager::Instance().keyPressed(SDL_SCANCODE_M))
	{
		m_shipIsMoving = !m_shipIsMoving;

		if (!m_isGridEnabled && m_playMoveSound)
		{
			SoundManager::Instance().playSound("marchsound", -1, 0);
		}
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Set SFX and Music
	SoundManager::Instance().load("../Assets/audio/mainBGM.mp3", "main", SOUND_MUSIC);
	SoundManager::Instance().playMusic("main", -1, 0);

	SoundManager::Instance().load("../Assets/audio/yay.ogg", "goalsound", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/thunder.ogg", "marchsound", SOUND_SFX);

	SoundManager::Instance().setAllVolume(5);


	m_buildGrid();

	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	m_currentHeuristic = MANHATTAN;
	
	m_pTarget = new Target(); // instantiating a new Target object - allocating memory on the Heap
	m_pTarget->getTransform()->position = m_getTile(15, 11)->getTransform()->position + offset;
	m_pTarget->setGridPosition(15, 11);
	m_getTile(15, 11)->setTileStatus(GOAL);
	addChild(m_pTarget);

	m_pSpaceShip = new SpaceShip();
	m_pSpaceShip->getTransform()->position = m_getTile(1, 3)->getTransform()->position + offset;
	m_pSpaceShip->setGridPosition(1, 3);
	m_getTile(1, 3)->setTileStatus(START);
	addChild(m_pSpaceShip);

	// Create Obstacle
	m_createObstacle(5, 5, offset);
	m_createObstacle(7, 5, offset);
	m_createObstacle(8, 9, offset);
	m_createObstacle(9, 9, offset);
	m_createObstacle(10, 7, offset);
	m_createObstacle(10, 8, offset);

	// Created random Obstacle
	m_createRandomObstacle();

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

	// setup neighbour refernece 
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
			
			// top right
			if (row == 0 || col == Config::COL_NUM - 1)
			{
				tile->setNeighbourTile(TOP_RIGHT_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(TOP_RIGHT_TILE, m_getTile(col + 1, row - 1));
			}

			// top left
			if (row == 0 || col == 0)
			{
				tile->setNeighbourTile(TOP_LEFT_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(TOP_LEFT_TILE, m_getTile(col - 1, row - 1));
			}

			// bottom right
			if (row == Config::ROW_NUM - 1 || col == Config::COL_NUM - 1)
			{
				tile->setNeighbourTile(BOTTOM_RIGHT_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(BOTTOM_RIGHT_TILE, m_getTile(col + 1, row + 1));
			}

			// bottom left
			if (row == Config::ROW_NUM - 1 || col == 0)
			{
				tile->setNeighbourTile(BOTTOM_LEFT_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(BOTTOM_LEFT_TILE, m_getTile(col - 1, row + 1));
			}
		}
	}
}

void PlayScene::m_createObstacle(int x, int y, glm::vec2 offset)
{
	m_obstacle = new Obstacle();
	m_pObstacle.push_back(m_obstacle);
	m_obstacle->getTransform()->position = m_getTile(x, y)->getTransform()->position + offset;
	m_obstacle->setGridPosition(x, y);
	m_getTile(x, y)->setTileStatus(IMPASSABLE);
	addChild(m_obstacle);
}

void PlayScene::m_createRandomObstacle()
{
	srand((unsigned)time(0));
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	int count = 0;
	Tile* pTile;

	while (count < 5)
	{
		pTile = m_getTile(rand() % Config::COL_NUM, rand() % Config::ROW_NUM);
		if (pTile->getTileStatus() == UNVISITED)
		{
			m_createObstacle(pTile->getGridPosition().x, pTile->getGridPosition().y, offset);
			count++;
		}
	}
}

void PlayScene::m_findShortestPath()
{
	// If there's no path, we're good to pathfind
	if (m_pPathList.empty())
	{
		// Step 1 - Add start position to the open list
		Tile* startTile = m_getTile(m_pSpaceShip->getGridPosition());
		startTile->setTileStatus(OPEN);
		m_pOpenList.push_back(startTile);

		bool goalFound = false; // Flag for while loop

		// Step 2 - Iterate until the open list is empty or the goal is found
		while (!m_pOpenList.empty() && !goalFound)
		{
			float min = INFINITY; // INFINITY == Max tile cost
			Tile* minTile;
			int minTileIndex = 0;
			int count = 0;

			std::vector<Tile*> neighbourList;
			for (int index = 0; index < NUM_OF_NEIGHBOUR_TILES; index++)
			{
				if (m_pOpenList[0]->getNeighbourTile(NeighbourTile(index)) == nullptr)
					continue;
				if (m_pOpenList[0]->getNeighbourTile(NeighbourTile(index))->getTileStatus() == IMPASSABLE)
					continue;

				neighbourList.push_back(m_pOpenList[0]->getNeighbourTile(NeighbourTile(index)));
			}

			for (Tile* neighbour : neighbourList)
			{
				if (neighbour->getTileStatus() != GOAL)
				{
					if (neighbour->getTileCost() < min)
					{
						min = neighbour->getTileCost();
						minTile = neighbour;
						minTileIndex = count;
					}
					count++;
				}
				else // We have found the goal, yay!
				{
					minTile = neighbour;
					m_pPathList.push_back(minTile);
					goalFound = true;
					break; // Stop the search!
				}
			}
			// Remove the reference of the current tile in the open list
			m_pPathList.push_back(m_pOpenList[0]);
			m_pOpenList.pop_back();
			// Add the minTile to the openList
			m_pOpenList.push_back(minTile);
			minTile->setTileStatus(OPEN);
			neighbourList.erase(neighbourList.begin() + minTileIndex);
			// Push all remaining neighbours onto the closed list
			for (Tile* neighbour : neighbourList)
			{
				if (neighbour->getTileStatus() == UNVISITED)
				{
					neighbour->setTileStatus(CLOSED);
					m_pClosedList.push_back(neighbour);
				}
			}
		}
		// Fixes the last two tiles being swapped
		Tile* goal = m_pPathList.at(m_pPathList.size() - 2);
		m_pPathList.erase(m_pPathList.end() - 2);
		m_pPathList.push_back(goal);
	}
}

void PlayScene::m_displayPathList()
{
	for (Tile* node : m_pPathList)
	{
		node->setEnabled(true);
		node->setLabelsEnabled(true);
		/*std::cout << "(" << node->getGridPosition().x << ", "
			<< node->getGridPosition().y << ")" << std::endl;*/
	}
	//std::cout << "Path Length: " << m_pPathList.size() << std::endl;
}

void PlayScene::m_resetPathfinding()
{
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	// Clear tile vectors
	m_pPathList.clear();
	m_pPathList.shrink_to_fit();
	m_pOpenList.clear();
	m_pOpenList.shrink_to_fit();
	m_pClosedList.clear();
	m_pClosedList.shrink_to_fit();

	// Reset tile statuses
	for (Tile* tile : m_pGrid)
	{
		tile->setTileStatus(UNVISITED);
	}

	// Reset target data
	//m_pTarget->getTransform()->position = m_getTile(15, 11)->getTransform()->position + offset;
	//m_pTarget->setGridPosition(15.0f, 11.0f);
	m_getTile(m_pTarget->getGridPosition())->setTileStatus(GOAL);
	//goal_position[0] = m_pTarget->getGridPosition().x;
	//goal_position[1] = m_pTarget->getGridPosition().y;
	
	// Reset ship data
	//m_pSpaceShip->getTransform()->position = m_getTile(1, 3)->getTransform()->position + offset;
	//m_pSpaceShip->setGridPosition(1.0f, 3.0f);
	m_getTile(m_pSpaceShip->getGridPosition())->setTileStatus(START);
	//start_position[0] = m_pSpaceShip->getGridPosition().x;
	//start_position[1] = m_pSpaceShip->getGridPosition().y;
	
	Obstacle* obstacle;
	for (int i = 0; i < m_pObstacle.size(); i++)
	{
		obstacle = m_pObstacle[i];
		m_getTile(obstacle->getGridPosition())->setTileStatus(IMPASSABLE);
	}


	m_moveCounter = 0;
	m_shipIsMoving = false;
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
		dx = m_pTarget->getGridPosition().x - tile->getGridPosition().x; // x2 - x1
		dy = m_pTarget->getGridPosition().y - tile->getGridPosition().y; // y2 - y1

		switch (m_currentHeuristic)
		{
		case MANHATTAN:
			distance = abs(dx) + abs(dy);
			break;
		case EUCLIDEAN:

			// computes euclidean distance ("as the crow flies") for each tile 
			distance = sqrt(dx * dx + dy * dy); // C^2 = A^2 + B^2
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

void PlayScene::m_moveShip()
{
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	if (m_moveCounter < m_pPathList.size())
	{
		m_pSpaceShip->getTransform()->position = m_getTile(m_pPathList[m_moveCounter]->getGridPosition())->getTransform()->position + offset;
		m_pSpaceShip->setGridPosition(m_pPathList[m_moveCounter]->getGridPosition().x, m_pPathList[m_moveCounter]->getGridPosition().y);

		m_pSpaceShip->setDesiredVelocity(m_getTile(m_pPathList[m_moveCounter]->getGridPosition())->getTransform()->position + offset);

		const glm::vec2 steering_direction = m_pSpaceShip->getDesiredVelocity() - m_pSpaceShip->getCurrentDirection();

		m_pSpaceShip->LookWhereYoureGoing(steering_direction);

		if (Game::Instance().getFrames() % 20 == 0)
		{
			m_moveCounter++;
		}
	}
	else
	{
		if (!m_isGridEnabled && m_playMoveSound)
		{
			SoundManager::Instance().playSound("goalsound", 0, 0);
			m_playMoveSound = !m_playMoveSound;
		}

		m_shipIsMoving = false;
	}
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

	if (ImGui::Button("Reset Play Scene"))
	{
		m_resetPathfinding();
		m_setGridEnabled(false);
		while (m_pObstacle.size() >= 6)
		{
			removeChild(m_pObstacle.at(m_pObstacle.size() - 1));
			m_pObstacle.pop_back();
		}
		m_createRandomObstacle();
	}

	ImGui::Separator();
	ImGui::Text("Total Cost");
	if (m_pPathList.size() > 0)
	{
		char array[10];
		sprintf(array, "%.1f", m_pPathList[0]->getTileCost());
		ImGui::Text(array);
	}

	ImGui::Separator();

	// Grid position properties
	start_position[0] = m_pSpaceShip->getGridPosition().x;
	start_position[1] = m_pSpaceShip->getGridPosition().y;
	if (ImGui::SliderInt2("Start Position", start_position, 0, Config::COL_NUM - 1))
	{
		// constrain object within max row
		if (start_position[1] > Config::ROW_NUM - 1)
		{
			start_position[1] = Config::ROW_NUM - 1;
		}

		m_getTile(m_pSpaceShip->getGridPosition())->setTileStatus(UNVISITED);
		m_pSpaceShip->getTransform()->position = m_getTile(start_position[0], start_position[1])->getTransform()->position + offset;
		m_pSpaceShip->setGridPosition(start_position[0], start_position[1]);
		m_getTile(m_pSpaceShip->getGridPosition())->setTileStatus(START);
	}

	// target properties

	goal_position[0] = m_pTarget->getGridPosition().x;
	goal_position[1] = m_pTarget->getGridPosition().y;
	if (ImGui::SliderInt2("Goal Position", goal_position, 0, Config::COL_NUM - 1))
	{
		// constrain object within max row
		if (goal_position[1] > Config::ROW_NUM - 1)
		{
			goal_position[1] = Config::ROW_NUM - 1;
		}

		m_getTile(m_pTarget->getGridPosition())->setTileStatus(UNVISITED);
		m_pTarget->getTransform()->position = m_getTile(goal_position[0], goal_position[1])->getTransform()->position + offset;
		m_pTarget->setGridPosition(goal_position[0], goal_position[1]);
		m_getTile(m_pTarget->getGridPosition())->setTileStatus(GOAL);
		m_computeTileCost();
	}
	
	ImGui::End();
}

int PlayScene::start_position[2];
int PlayScene::goal_position[2];