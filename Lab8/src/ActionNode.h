#pragma once
#ifndef __ACTION_NODE__
#define __ACTION_NODE__
#include "TreeNode.h"
#include "Agent.h"

// Interface
class ActionNode : public TreeNode
{
public:
	ActionNode() { m_isLeaf = true; }
	virtual ~ActionNode() = default;
	virtual void Action() = 0;
};

#endif /* defined (__ACTION_NODE__) */