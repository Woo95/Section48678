#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
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

	Plane* m_pPlaneSprite;
	bool m_playerFacingRight;

	// UI Items
	Label* m_pInstructionsLabel;
	Label* m_pInstructionsLabel1;
	Label* m_pInstructionsLabel2;
};

#endif /* defined (__PLAY_SCENE__) */