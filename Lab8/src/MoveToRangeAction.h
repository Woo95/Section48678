#pragma once
#ifndef __MOVE_TO_RANGE_ACTION__
#define __MOVE_TO_RANGE_ACTION__
#include "ActionNode.h"

class MoveToRangeAction : public ActionNode
{
public:
	MoveToRangeAction();
	virtual ~MoveToRangeAction();

	virtual void Action() override;
private:

};

#endif 
