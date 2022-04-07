#pragma once
#ifndef __TARGET__
#define __TARGET__

#include "Agent.h"

class Target final : public virtual Agent
{
public:
	Target();
	~Target();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

private:
	void m_move();
	void m_checkBounds();
	void m_reset();
};

#endif /* defined (__TARGET__) */