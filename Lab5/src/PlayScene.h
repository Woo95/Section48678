#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Heuristic.h"
#include "Scene.h"
#include "Target.h"
#include "SpaceShip.h"
#include "Obstacle.h"
#include "Tile.h"

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
	static int start_position[2];
	static int goal_position[2];


	// Game Objects
	Target* m_pTarget;
	SpaceShip* m_pSpaceShip;
	Obstacle* m_obstacle;

	// Pathfinding Objects and Functions
	std::vector<Tile*> m_pGrid;
	void m_buildGrid();
	void m_setGridEnabled(bool state);
	bool m_getGridEnabled() const;
	void m_computeTileCosts();
	void m_createObstacle(int, int, glm::vec2);

	void m_findShortestPath(); // Our big algorithm
	void m_displayPathList(); // In console
	void m_resetPathfinding();

	// Tile lists for pathfinding
	std::vector<Tile*> m_pOpenList;
	std::vector<Tile*> m_pClosedList;
	std::vector<Tile*> m_pPathList;

	// convenience functions
	Tile* m_getTile(int col, int row);
	Tile* m_getTile(glm::vec2 grid_position);

	// heuristic
	Heuristic m_currentHeuristic;

	// Ship movement
	int m_moveCounter = 0;
	bool m_shipIsMoving = false;
	void m_moveShip();
};

#endif /* defined (__PLAY_SCENE__) */