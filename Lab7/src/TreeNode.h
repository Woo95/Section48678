#pragma once
#ifndef __TREE_NODE__
#define __TREE_NODE__
#include <string>

struct TreeNode
{
	TreeNode() = default;
	virtual ~TreeNode() = default;

	std::string m_name = "";

	TreeNode* m_pLeft = nullptr;	// False child
	TreeNode* m_pRight = nullptr;	// True child
	TreeNode* m_pParent = nullptr;
	bool m_isLeaf = false;
};

#endif /* defined (__TREE_NODE__) */
