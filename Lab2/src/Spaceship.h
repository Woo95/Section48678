#pragma once
#ifndef __SPACE_SHIP__
#define __SPACE_SHIP__

#include "DisplayObject.h"

class SpaceShip final : public DisplayObject
{
public:
	SpaceShip();
	~SpaceShip();

	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

private:
};

#endif /* defined (__SPACE_SHIP__) */