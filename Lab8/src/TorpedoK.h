#pragma once
#ifndef __TORPEDO_K__
#define __TORPEDO_K__

#include "TorpedoAnimationState.h"
#include "Sprite.h"

class TorpedoK final : public virtual Sprite
{
public:
	TorpedoK(float speed = 0.0f, glm::vec2 direction = {0,0});
	~TorpedoK();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	// setters
	void setAnimationState(TorpedoAnimationState new_state);

private:
	void m_buildAnimations();

	TorpedoAnimationState m_currentAnimationState;

	glm::vec2 m_direction;
};

#endif /* defined (__TORPEDO_K__) */