#pragma once
#ifndef __PATH__NODE__
#define __PATH__NODE__
#include "Agent.h"

class PathNode : public Agent
{
public:
	PathNode();
	~PathNode();

	// DisplayObject Life-Cycle Functions
	void draw() override;
	void update() override;
	void clean() override;
};



#endif