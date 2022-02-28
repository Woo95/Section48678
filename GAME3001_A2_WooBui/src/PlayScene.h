#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Target.h"
#include "SpaceShip.h"
#include "Label.h"
#include "Tile.h"
#include "Heuristic.h"
#include "Obstacle.h"

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

	Obstacle* m_obstacle;

	Target* m_pTarget;

	SpaceShip* m_pSpaceShip;
	bool m_playerFacingRight;

	// pathfinding objects and function
	std::vector<Tile*> m_pGrid;
	std::vector<Obstacle*> m_pObstacle;
	bool m_isGridEnabled = false;

	static int start_position[2];
	static int goal_position[2];

	void m_buildGrid();
	bool m_getGridEnabled() const;
	void m_setGridEnabled(bool state);
	void m_computeTileCost();
	void m_createObstacle(int, int, glm::vec2);
	void m_createRandomObstacle();

	void m_findShortestPath(); // Our big algorithm
	void m_displayPathList(); // In console
	void m_resetPathfinding();

	// Tile lists for pathfinding
	std::vector<Tile*> m_pOpenList;
	std::vector<Tile*> m_pClosedList;
	std::vector<Tile*> m_pPathList;

	// convert world to grid
	Tile* m_getTile(int col, int row);
	Tile* m_getTile(glm::vec2 grid_position);

	// Heuristic tracking
	Heuristic m_currentHeuristic;

	// UI Items
	Label* m_pInstructionsLabel;
	Label* m_pInstructionsLabel1;
	Label* m_pInstructionsLabel2;
	Label* m_pInstructionsLabel3;

	// Ship movement
	int m_moveCounter = 0;
	bool m_shipIsMoving = false;
	void m_moveShip();
	bool m_playMoveSound = false;
};

#endif /* defined (__PLAY_SCENE__) */