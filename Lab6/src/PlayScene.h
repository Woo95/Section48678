#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Heuristic.h"
#include "Scene.h"
#include "Target.h"
#include "SpaceShip.h"
#include "Obstacle.h"
#include "PathNode.h"
#include <ctime>

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	glm::vec2 m_mousePosition;
	bool m_isGridEnabled;


	// Game Objects
	Target* m_pTarget;
	SpaceShip* m_pSpaceShip;
	Obstacle* m_pObstacle1;
	Obstacle* m_pObstacle2;
	Obstacle* m_pObstacle3;

	// Pathfinding Objects and Functions
	std::vector<PathNode*> m_pGrid;
	std::vector<Obstacle*> m_pObstacle;
	void m_buildGrid();
	void m_toggleGrid(bool state);
	void m_CheckShipLOS(DisplayObject* target_object);
	
	// convenience functions
	void m_storeObstacles();
	void m_clearNodes();

	static int m_ObstacleBuffer;
};

#endif /* defined (__PLAY_SCENE__) */