#pragma once
#ifndef __TREE_NODE__
#define __TREE_NODE__
#include <string>
#include "Agent.h"

struct TreeNode
{
	TreeNode()
	{
		m_agent = nullptr;
	}
	virtual ~TreeNode() = default;

	std::string m_name = "";

	TreeNode* m_pLeft = nullptr; // False child
	TreeNode* m_pRight = nullptr; // True child
	TreeNode* m_pParent = nullptr;
	bool m_isLeaf = false;

	Agent* getAgent() { return m_agent; }
	void setAgent(Agent* agent) { m_agent = agent; }

protected:
	Agent* m_agent;
};

#endif /* defined (__TREE_NODE__) */
