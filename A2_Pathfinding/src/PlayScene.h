#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Target.h"
#include "SpaceShip.h"
#include "Button.h"
#include "Label.h"

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
	void GUI_Function() const;
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	Target* m_target;

	SpaceShip* m_spaceShip;
	bool m_playerFacingRight;

	// UI Items
	Label* m_pInstructionsLabel;
	Label* m_pInstructionsLabel1;
	Label* m_pInstructionsLabel2;
};

#endif /* defined (__PLAY_SCENE__) */