#pragma once
#ifndef __LOS_CONDITION__
#define __LOS_CONDITION__
#include "ConditionNode.h"

class LOSCondition : public ConditionNode
{
public:
	LOSCondition(bool LOS = false);
	virtual ~LOSCondition();

	// Getters and Setters

	virtual bool Condition() override;
};

#endif