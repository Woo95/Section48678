#include "DecisionTree.h"
#include <iostream>
#include "AttackAction.h"
#include "MoveToLOSAction.h"
#include "MoveToPlayerAction.h"
#include "PatrolAction.h"

DecisionTree::DecisionTree() = default;

DecisionTree::DecisionTree(Agent* agent)
{
	m_agent = agent;
}

DecisionTree::~DecisionTree()
= default;

// Getters and Setters

Agent* DecisionTree::getAgent() const
{
	return m_agent;
}

LOSCondition* DecisionTree::getLOSNode() const
{
	return m_LOSNode;
}

RadiusCondition* DecisionTree::getRadiusNode() const
{
	return m_RadiusNode;
}

CloseCombatCondition* DecisionTree::getCloseCombatNode() const
{
	return m_CloseCombatNode;
}

std::vector<TreeNode*>& DecisionTree::getTree()
{
	return m_treeNodeList;
}

void DecisionTree::setLOSNode(LOSCondition* node)
{
	m_LOSNode = node;
}

void DecisionTree::setRadiusNode(RadiusCondition* node)
{
	m_RadiusNode = node;

}

void DecisionTree::setCloseCombatNode(CloseCombatCondition* node)
{
	m_CloseCombatNode = node;

}

void DecisionTree::setAgent(Agent* agent)
{
	m_agent = agent;
}

// Add node
TreeNode* DecisionTree::AddNode(TreeNode* parent, TreeNode* child_node, const TreeNodeType type)
{
	switch (type)
	{
	case LEFT_TREE_NODE:
		parent->m_pLeft = child_node;
		break;

	case RIGHT_TREE_NODE:
		parent->m_pRight = child_node;
		break;
	}
	child_node->m_pParent = parent;
	return child_node;
}

void DecisionTree::Display()
{
	for (auto* node : m_treeNodeList)
	{
		std::cout << node->m_name << std::endl;
	}
}

void DecisionTree::Update()
{
	// Do some checks here or in PlayScene...
}

void DecisionTree::clean()
{

	// Clear all the nodes.
	for (auto node : m_treeNodeList)
	{
		delete node;
		node = nullptr;
	}
	m_treeNodeList.clear();
	m_treeNodeList.shrink_to_fit();

	// Wrangle the remaining pointers. Optional.
	m_LOSNode = nullptr;
	m_RadiusNode = nullptr;
	m_CloseCombatNode = nullptr;
}

// Make decision

// in-order traversal
void DecisionTree::MakeDecision()
{
	auto currentNode = m_treeNodeList[0]; // Start at root node.
	while (!currentNode->m_isLeaf)
	{
		currentNode = dynamic_cast<ConditionNode*>(currentNode)->Condition() ? (currentNode->m_pRight) : (currentNode->m_pLeft);
	}
	static_cast<ActionNode*>(currentNode)->Action();
}
