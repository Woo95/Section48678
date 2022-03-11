#pragma once
#ifndef __OBSTACLE__
#define __OBSTACLE__
#include "DisplayObject.h"

class Obstacle final : public DisplayObject
{
public:
	// constructors
	Obstacle();

	// destructor
	~Obstacle();

	// life cycle functions
	void draw() override;
	void update() override;
	void clean() override;

	// getters and setters
	glm::vec2 getGridPosition() const;
	void setGridPosition(float col, float row);
private:
	glm::vec2 m_gridPosition;
};

#endif /* defined (__OBSTACLE__) */