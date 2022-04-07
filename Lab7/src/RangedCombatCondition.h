#pragma once
#ifndef __RANGED_COMBAT_CONDITION__
#define __RANGED_COMBAT_CONDITION__
#include "ConditionNode.h"

class RangedCombatCondition : public ConditionNode
{
public:
	RangedCombatCondition(bool within_combat_range = false);
	virtual ~RangedCombatCondition();

	// Getters and Setters
	void setIsWithinCombatRange(bool state);

	virtual bool Condition() override;
private:
	bool m_isWithinCombatRange;
};

#endif /* defined (__RANGED_COMBAT_CONDITION__) */