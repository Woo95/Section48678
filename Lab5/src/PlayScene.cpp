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
	updateDisplayList(); //comment for now
	if (m_shipIsMoving)
	{
		m_moveShip();
	}
	//if(m_pSpaceShip->isEnabled())
	//{
	//	
	//	CollisionManager::circleAABBCheck(m_pTarget, m_pSpaceShip);
	//	CollisionManager::rotateAABB(m_pSpaceShip, m_pSpaceShip->getCurrentHeading());
	//}
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

	//Setup the grid
	m_buildGrid();
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	m_currentHeuristic = MANHATTAN;

	m_pTarget = new Target(); // instantiating a new Target object - allocating memory on the Heap
	m_pTarget->getTransform()->position = m_getTile(15, 11)->getTransform()->position + offset;
	m_pTarget->setGridPosition(15.0f, 11.0f);
	m_getTile(15, 11)->setTileStatus(GOAL);
	addChild(m_pTarget);

	m_pSpaceShip = new SpaceShip();
	m_pSpaceShip->getTransform()->position = m_getTile(1, 3)->getTransform()->position + offset;
	// position in world space matches grid space
	m_pSpaceShip->setGridPosition(1.0f, 3.0f);
	m_getTile(1, 3)->setTileStatus(START);
	addChild(m_pSpaceShip);

	SoundManager::Instance().load("../Assets/audio/yay.ogg", "yay", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/thunder.ogg", "boom", SOUND_SFX);

	m_computeTileCosts();

	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function()
{
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Lab 4 Debug Properties", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::Separator();

	static bool toggleGrid = false;
	if (ImGui::Checkbox("Toggle Grid", &toggleGrid))
	{
		m_isGridEnabled = toggleGrid;
		m_setGridEnabled(m_isGridEnabled);
	}

	ImGui::Separator();

	static int radio = static_cast<int>(m_currentHeuristic);
	ImGui::Text("Heuristic Type");
	ImGui::RadioButton("Manhattan", &radio, static_cast<int>(MANHATTAN));
	ImGui::SameLine();
	ImGui::RadioButton("Euclidean", &radio, static_cast<int>(EUCLIDEAN));

	if (m_currentHeuristic != static_cast<Heuristic>(radio))
	{
		m_currentHeuristic = static_cast<Heuristic>(radio);
		m_computeTileCosts();
	}

	ImGui::Separator();
	if (ImGui::Button("Find Shortest Path"))
	{
		m_findShortestPath();
	}

	ImGui::Separator();
	if (ImGui::Button("Start"))
	{
		if (!m_shipIsMoving)
		{
			m_shipIsMoving = true;
		}
	}

	ImGui::Separator();
	if (ImGui::Button("Reset"))
	{
		m_resetPathfinding();
	}

	// Grid Position properties
	start_position[0] = m_pSpaceShip->getGridPosition().x;
	start_position[1] = m_pSpaceShip->getGridPosition().y;
	if (ImGui::SliderInt2("Start Position", start_position, 0, Config::COL_NUM - 1))
	{
		if (start_position[1] > Config::ROW_NUM - 1)
		{
			start_position[1] = Config::ROW_NUM - 1;
		}

		m_getTile(m_pSpaceShip->getGridPosition())->setTileStatus(UNVISITED);
		m_pSpaceShip->getTransform()->position = m_getTile(start_position[0], start_position[1])->getTransform()->position + offset;
		m_pSpaceShip->setGridPosition(start_position[0], start_position[1]);
		m_getTile(m_pSpaceShip->getGridPosition())->setTileStatus(START);

	}

	goal_position[0] = m_pTarget->getGridPosition().x;
	goal_position[1] = m_pTarget->getGridPosition().y;
	if (ImGui::SliderInt2("Goal Position", goal_position, 0, Config::COL_NUM - 1))
	{
		if (goal_position[1] > Config::ROW_NUM - 1)
		{
			goal_position[1] = Config::ROW_NUM - 1;
		}

		m_getTile(m_pTarget->getGridPosition())->setTileStatus(UNVISITED);
		m_pTarget->getTransform()->position = m_getTile(goal_position[0], goal_position[1])->getTransform()->position + offset;
		m_pTarget->setGridPosition(goal_position[0], goal_position[1]);
		m_getTile(m_pTarget->getGridPosition())->setTileStatus(GOAL);
		m_computeTileCosts();
	}

	ImGui::End();
}

void PlayScene::m_buildGrid()
{
	auto tileSize = Config::TILE_SIZE;

	// add tiles to the grid

	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			Tile* tile = new Tile(); // create a new empty tile
			tile->getTransform()->position = glm::vec2(col * tileSize, row * tileSize); // world position
			tile->setGridPosition(col, row);
			addChild(tile);
			tile->addLabels();
			tile->setEnabled(false);
			m_pGrid.push_back(tile);
		}
	}
	// create references (connections) for each tile to its neighbours (N, S, E, W)
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			Tile* tile = m_getTile(col, row);

			// If at topmost row
			if (row == 0)
			{
				tile->setNeighbourTile(TOP_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(TOP_TILE, m_getTile(col, row - 1));
			}
			// If at rightmost col
			if (col == Config::COL_NUM - 1)
			{
				tile->setNeighbourTile(RIGHT_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(TOP_TILE, m_getTile(col + 1, row));
			}
			// If at bottomost row
			if (row == Config::ROW_NUM -1)
			{
				tile->setNeighbourTile(BOTTOM_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(BOTTOM_TILE, m_getTile(col, row+1));
			}
			// If at leftmost row
			if (col == 0)
			{
				tile->setNeighbourTile(LEFT_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(LEFT_TILE, m_getTile(col -1, row));
			}
		}
	}
}

void PlayScene::m_setGridEnabled(bool state)
{
	m_isGridEnabled = state;

	for (auto tile : m_pGrid)
	{
		tile->setEnabled(m_isGridEnabled); //game object
		tile->setLabelsEnabled(m_isGridEnabled); //tile class
	}
}

bool PlayScene::m_getGridEnabled() const
{
	return m_isGridEnabled;
}

void PlayScene::m_computeTileCosts()
{
	float distance = 0.0f; // Total distance

	float dx = 0.0f;
	float dy = 0.0f;

	for (auto tile : m_pGrid)
	{
		dx = m_pTarget->getGridPosition().x - tile->getGridPosition().x; // x2 - x1
		dy = m_pTarget->getGridPosition().y - tile->getGridPosition().y; // y2 - y1
		switch (m_currentHeuristic)
		{
		case MANHATTAN:
			distance = abs(dx) + abs(dy);
			break;
		case EUCLIDEAN:
			distance = sqrt(dx * dx + dy * dy); // C^2 = A^2 + B^2
			break;
		}
		tile->setTileCost(distance);
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

		bool goalFound = false; // Flag for while loop.

		// Step 2- Iterate until the open list is empty or the goal is found
		while (!m_pOpenList.empty() && !goalFound)
		{
			float min = INFINITY; // INFINITY == Max tile cost
			Tile* minTile;
			int minTileIndex = 0;
			int count = 0;

			std::vector<Tile*> neighbourList;
			for (int index = 0; index < NUM_OF_NEIGHBOUR_TILES; index++)
			{
				neighbourList.push_back(m_pOpenList[0]->getNeighbourTile(NeighbourTile(index)));
			}
			for (Tile* neighbour : neighbourList)
			{
				if (neighbour->getTileStatus() != GOAL)
				{
					if (neighbour->getTileStatus() < min)
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
}

void PlayScene::m_resetPathfinding()
{
}

Tile* PlayScene::m_getTile(int col, int row)
{
	return m_pGrid[(row * Config::COL_NUM) + col];
}

Tile* PlayScene::m_getTile(glm::vec2 grid_position)
{
	const auto col = grid_position.x;
	const auto row = grid_position.y;

	return m_pGrid[(row * Config::COL_NUM) + col];
}

void PlayScene::m_moveShip()
{
}

int PlayScene::start_position[2];
int PlayScene::goal_position[2];
