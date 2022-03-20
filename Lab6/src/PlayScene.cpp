#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include <fstream>

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
	m_checkAgentLOS(m_pSpaceShip, m_pTarget);
	// for path_nodes LOS
	switch (m_LOSMode)
	{
	case 0:
		m_checkAllNodesWithTarget(m_pTarget);
		break;
	case 1:
		m_checkAllNodesWithTarget(m_pSpaceShip);
		break;
	case 2:
		m_checkAllNodesWithBoth();
		break;
	// for finding path
	case 3:
		m_checkNodeAgentToTargetPath();
		break;
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

	//New Obstacle creation
	std::ifstream inFile("../Assets/Data/obstacles.txt");
	while (!inFile.eof())
	{
		Obstacle* obstacle = new Obstacle();
		float x, y, w, h;
		inFile >> x >> y >> w >> h;
		obstacle->getTransform()->position = glm::vec2(x, y);
		obstacle->setWidth(w);
		obstacle->setHeight(h);
		addChild(obstacle);
		m_pObstacles.push_back(obstacle);
	}
	inFile.close();

	m_pTarget = new Target(); // instantiating a new Target object - allocating memory on the Heap
	m_pTarget->getTransform()->position = glm::vec2(600.0f, 300.0f);
	addChild(m_pTarget);

	m_pSpaceShip = new SpaceShip();
	m_pSpaceShip->getTransform()->position = glm::vec2(150.0f, 300.0f);
	addChild(m_pSpaceShip, 3);

	//Setup field
	m_LOSMode = 0;
	m_ObstacleBuffer = 0;
	m_pathNodeLOSDistance = 1000;
	m_setPathNodeLOSDistance(m_pathNodeLOSDistance);

	//Setup the grid
	m_isGridEnabled = false;
	m_buildGrid();
	m_toggleGrid(m_isGridEnabled);

	SoundManager::Instance().load("../Assets/audio/yay.ogg", "yay", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/thunder.ogg", "boom", SOUND_SFX);

	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Lab 6 Debug Properties", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::Separator();

	if (ImGui::Checkbox("Toggle Grid", &m_isGridEnabled))
	{
		m_toggleGrid(m_isGridEnabled);
	}

	ImGui::Separator();

	if (ImGui::Button("Node LOS to Target", { 300, 20 }))
	{
		m_LOSMode = 0;
	}
	if (m_LOSMode == 0)
	{
		ImGui::SameLine();
		ImGui::Text("<Active>");
	}

	if (ImGui::Button("Node LOS to SpaceShip", { 300, 20 }))
	{
		m_LOSMode = 1;
	}
	if (m_LOSMode == 1)
	{
		ImGui::SameLine();
		ImGui::Text("<Active>");
	}

	if (ImGui::Button("Node LOS to Both Target and SpaceShip", { 300, 20 }))
	{
		m_LOSMode = 2;
	}
	if (m_LOSMode == 2)
	{
		ImGui::SameLine();
		ImGui::Text("<Active>");
	}

	// fourth button
	if (ImGui::Button("Draw SpaceShip to Target path", { 300, 20 }))
	{
		m_LOSMode = 3;
	}
	if (m_LOSMode == 3)
	{
		ImGui::SameLine();
		ImGui::Text("<Active>");
	}

	if (ImGui::SliderInt("Path Node LOS Distance", &m_pathNodeLOSDistance, 0, 1000))
	{
		m_setPathNodeLOSDistance(m_pathNodeLOSDistance);
	}

	ImGui::Separator();

	static int shipPosition[] = { m_pSpaceShip->getTransform()->position.x, m_pSpaceShip->getTransform()->position.y };
	if (ImGui::SliderInt2("Ship Position", shipPosition, 0, 800))
	{
		m_pSpaceShip->getTransform()->position.x = shipPosition[0];
		m_pSpaceShip->getTransform()->position.y = shipPosition[1];
	}

	//Allow ship rotation
	static int angle;
	if (ImGui::SliderInt("Ship Direction", &angle, -360, 360))
	{
		m_pSpaceShip->setCurrentHeading(angle);
	}

	ImGui::Separator();

	static int targetPosition[] = { m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y };
	if (ImGui::SliderInt2("Target Position", targetPosition, 0, 800))
	{
		m_pTarget->getTransform()->position.x = targetPosition[0];
		m_pTarget->getTransform()->position.y = targetPosition[1];
	}

	ImGui::Separator();

	for (unsigned i = 0; i < m_pObstacles.size(); i++)
	{
		int obsPosition[] = { m_pObstacles[i]->getTransform()->position.x, m_pObstacles[i]->getTransform()->position.y };
		std::string label = "Obstacle" + std::to_string(i + 1) + " Position";
		if (ImGui::SliderInt2(label.c_str(), obsPosition, 0, 800))
		{
			m_pObstacles[i]->getTransform()->position.x = obsPosition[0];
			m_pObstacles[i]->getTransform()->position.y = obsPosition[1];
			m_buildGrid();

		}
	}

	ImGui::Separator();

	if (ImGui::SliderInt("Obstacle Buffer", &m_ObstacleBuffer, 0, 100))
	{
		m_buildGrid();
	}

	ImGui::End();
}

void PlayScene::m_buildGrid()
{
	auto tileSize = Config::TILE_SIZE;
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	m_clearNodes(); //Because we often rebuild

	//Add pathnodes to the grid
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			PathNode* path_node = new PathNode();
			path_node->getTransform()->position = glm::vec2((col * tileSize) + offset.x, (row * tileSize) + offset.y);
			bool keepNode = true;
			for (auto obstacle : m_pObstacles)
			{
				if (CollisionManager::AABBCheckWithBuffer(path_node, obstacle, m_ObstacleBuffer))
				{
					keepNode = false; // We have collision between node and an obstacle
				}
			}
			if (keepNode)
			{
				addChild(path_node);
				m_pGrid.push_back(path_node);
			}
			else delete path_node;
		}
	}
	// if Grid is supposed to be hidden, make it so!
	m_toggleGrid(m_isGridEnabled);
}

void PlayScene::m_toggleGrid(bool state)
{
	for (auto path_node : m_pGrid)
	{
		path_node->setVisible(state);
	}
}

bool PlayScene::m_checkAgentLOS(Agent* agent, DisplayObject* target_object)
{
	bool hasLOS = false;
	agent->setHasLOS(hasLOS);
	// if ship to target distance is less than or equal to LOS Distance
	auto AgentToTargetDistance = Util::getClosestEdge(agent->getTransform()->position, target_object);
	if (AgentToTargetDistance <= agent->getLOSDistance())
	{
		std::vector<DisplayObject*> contactList;
		for (auto object : getDisplayList())
		{
			auto AgentToObjectDistance = Util::getClosestEdge(agent->getTransform()->position, object);
			if (AgentToObjectDistance > AgentToTargetDistance) continue;
			if ((object->getType() != AGENT) && (object->getType() != PATH_NODE) && (object->getType() != TARGET))
			{
				contactList.push_back(object);
			}
		}
		const glm::vec2 agentEndPoint = agent->getTransform()->position + agent->getCurrentDirection() * agent->getLOSDistance();
		hasLOS = CollisionManager::LOSCheck(agent, agentEndPoint, contactList, target_object);
		agent->setHasLOS(hasLOS);
	}
	return hasLOS;
}

bool PlayScene::m_checkPathNodeLOS(PathNode * path_node, DisplayObject * target_object)
{
	//Check angle to target so we can still use LOS distance for path_nodes
	auto targetDirection = target_object->getTransform()->position - path_node->getTransform()->position;
	auto normalizedDirection = Util::normalize(targetDirection);
	path_node->setCurrentDirection(normalizedDirection);

	return m_checkAgentLOS(path_node, target_object);
}

void PlayScene::m_checkAllNodesWithTarget(DisplayObject * target_object)
{
	for (auto path_node : m_pGrid)
	{
		m_checkPathNodeLOS(path_node, target_object);
	}
}

void PlayScene::m_checkAllNodesWithBoth()
{
	for (auto path_node : m_pGrid)
	{
		bool LOSWithSpaceShip = m_checkPathNodeLOS(path_node, m_pSpaceShip);
		bool LOSWithTarget = m_checkPathNodeLOS(path_node, m_pTarget);
		path_node->setHasLOS((LOSWithSpaceShip && LOSWithTarget ? true : false));
	}
}

void PlayScene::m_checkNodeAgentToTargetPath()
{
	glm::vec2 agentPoint = m_pSpaceShip->getTransform()->position;
	glm::vec2 targetPoint = m_pTarget->getTransform()->position;

	float pathDistance = 999999;
	float midPathPointAgentDistance = 0;
	float midPathPointTargetDistance = 0;
	PathNode* shortestPathNode = nullptr;
	bool isFind = false;
	for (auto path_node : m_pGrid)
	{
		bool LOSWithSpaceShip = m_checkPathNodeLOS(path_node, m_pSpaceShip);
		bool LOSWithTarget = m_checkPathNodeLOS(path_node, m_pTarget);

		path_node->setHasLOS(false);
		path_node->setLOSColour(glm::vec4(1, 0, 0, 1));

		if (LOSWithSpaceShip && LOSWithTarget)
		{
			midPathPointAgentDistance = Util::distance(agentPoint, path_node->getTransform()->position);
			midPathPointTargetDistance = Util::distance(targetPoint, path_node->getTransform()->position);
			
			if (pathDistance > midPathPointAgentDistance + midPathPointTargetDistance)
			{
				pathDistance = midPathPointAgentDistance + midPathPointTargetDistance;
				shortestPathNode = path_node;
				isFind = true;
			}
		}
	}

	if (isFind)
	{
		float nodeSize = 40.0f;
		auto targetOffset = glm::vec2(nodeSize * 0.5f, nodeSize * 0.5f);
		glm::vec2 pathMidPoint = shortestPathNode->getTransform()->position;

		for (auto path_node : m_pGrid)
		{
			// Detect AgentPointLOS to PathMidPoint
			if (CollisionManager::lineRectCheck(agentPoint, pathMidPoint, path_node->getTransform()->position - targetOffset,
				nodeSize, nodeSize))
			{
				path_node->setHasLOS(true);
				path_node->setLOSColour(glm::vec4(0, 0, 1, 1));
			}

			// Detect TargetPointLOS to PathMidPoint
			if (CollisionManager::lineRectCheck(targetPoint, pathMidPoint, path_node->getTransform()->position - targetOffset,
				nodeSize, nodeSize))
			{
				path_node->setHasLOS(true);
				path_node->setLOSColour(glm::vec4(0, 0, 1, 1));
			}
		}
	}
	else std::cout << "there is no possible path\n";

	if (isFind)
	{
		glm::vec2 pathMidPoint = shortestPathNode->getTransform()->position;

		auto targetToPathMidPointDirection = pathMidPoint - targetPoint;
		auto agentToPathMidPointDirection = pathMidPoint - agentPoint;

		// normalize vector
		targetToPathMidPointDirection = Util::normalize(targetToPathMidPointDirection);
		agentToPathMidPointDirection = Util::normalize(agentToPathMidPointDirection);

		if (abs(targetToPathMidPointDirection.x) == abs(agentToPathMidPointDirection.x) 
			&& abs(targetToPathMidPointDirection.y) == abs(agentToPathMidPointDirection.y))
		{
			std::cout << "the path is clear\n";
		}
	}
}

void PlayScene::m_setPathNodeLOSDistance(int dist)
{
	for (auto path_node : m_pGrid)
	{
		path_node->setLOSDistance((float)dist);
	}
}

void PlayScene::m_clearNodes()
{
	m_pGrid.clear();
	for (auto object : getDisplayList()) //Alternately could iterate through m_pGrid and removeChild()
	{
		if (object->getType() == PATH_NODE)
			removeChild(object); //Does the deallocation of objects through pointers
	}
}