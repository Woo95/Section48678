#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Target.h"
#include "SpaceShip.h"
#include "Label.h"
#include "Tile.h"
#include "Heuristic.h"

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

	Target* m_target;

	SpaceShip* m_spaceShip;
	bool m_playerFacingRight;

	// pathfinding objects and function
	std::vector<Tile*> m_pGrid;
	bool m_isGridEnabled = false;

	void m_buildGrid();
	bool m_getGridEnabled() const;
	void m_setGridEnabled(bool state);
	void m_computeTileCost();

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
};

#endif /* defined (__PLAY_SCENE__) */