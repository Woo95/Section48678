#pragma once
#ifndef __DECISION_TREE__
#define __DECISION_TREE__
#include "Agent.h"
#include "CloseCombatCondition.h"
#include "RangedCombatCondition.h"
#include "LOSCondition.h"
#include "RadiusCondition.h"
// New Lab 7c
#include "EnemyHealthCondition.h"
#include "EnemyHitCondition.h"
#include "PlayerDetectedCondition.h"
//
#include "TreeNode.h"
#include "TreeNodeType.h"

class DecisionTree
{
public:
	// constructor and destructor
	DecisionTree();
	DecisionTree(Agent* agent); // New overload for Lab 7.
	~DecisionTree();

	// getters and setters
	Agent* getAgent() const;
	LOSCondition* getLOSNode() const;
	RadiusCondition* getRadiusNode() const;
	CloseCombatCondition* getCloseCombatNode() const;
	RangedCombatCondition* getRangedCombatNode() const;
	// New Lab 7c
	EnemyHealthCondition* getEnemyHealthNode() const;
	EnemyHitCondition* getEnemyHitNode() const;
	PlayerDetectedCondition* getPlayerDetectedNode() const;
	//
	std::vector<TreeNode*>& getTree();

	void setLOSNode(LOSCondition* node);
	void setRadiusNode(RadiusCondition* node);
	void setCloseCombatNode(CloseCombatCondition* node);
	void setRangedCombatNode(RangedCombatCondition* node);
	// New Lab 7c
	void setEnemyHealthNode(EnemyHealthCondition* node);
	void setEnemyHitNode(EnemyHitCondition* node);
	void setPlayerDetectedNode(PlayerDetectedCondition* node);
	//
	void setAgent(Agent* agent);

	// convenience functions
	TreeNode* AddNode(TreeNode* parent, TreeNode* child_node, TreeNodeType type);
	void Display();
	void Update();
	void clean();

	void MakeDecision(); // in order traversal
private:
	Agent* m_agent;

	LOSCondition* m_LOSNode;
	RadiusCondition* m_RadiusNode;
	CloseCombatCondition* m_CloseCombatNode;
	RangedCombatCondition* m_RangedCombatNode;
	// New Lab 7c
	EnemyHealthCondition* m_EnemyHealthNode;
	EnemyHitCondition* m_EnemyHitNode;
	PlayerDetectedCondition* m_PlayerDetectedNode;
	//
	std::vector<TreeNode*> m_treeNodeList;
};

#endif /* defined (__DECISION_TREE__) */