#pragma once
#ifndef __TORPEDO__
#define __TORPEDO__

#include "TorpedoAnimationState.h"
#include "Sprite.h"

class Torpedo final : public virtual Sprite
{
public:
	Torpedo(float speed = 0.0f);
	~Torpedo();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	// setters
	void setAnimationState(TorpedoAnimationState new_state);

private:
	void m_buildAnimations();

	TorpedoAnimationState m_currentAnimationState;

	float m_speed;
};

#endif /* defined (__PLAYER__) */