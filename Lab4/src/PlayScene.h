#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Target.h"
#include "SpaceShip.h"
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

	// Game Objects
	Target* m_pTarget;
	SpaceShip* m_pSpaceShip;

	// Pathfinding Objects and Functions
	std::vector<Tile*> m_pGrid;
	bool m_isGridEnabled;

	void m_buildGrid();
	void m_setGridEnabled(bool state);
	bool m_getGridEnabled() const;

	// convenience functions
	Tile* m_getTile(int col, int row);
	Tile* m_getTile(glm::vec2 grid_position);
};

#endif /* defined (__PLAY_SCENE__) */