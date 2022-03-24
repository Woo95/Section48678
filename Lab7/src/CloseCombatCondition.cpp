#include "CloseCombatCondition.h"


CloseCombatCondition::CloseCombatCondition(const bool within_combat_range)
{
	setIsWithinCombatRange(within_combat_range);
	m_name = "Close Combat Range Condition";
}

CloseCombatCondition::~CloseCombatCondition()= default;

void CloseCombatCondition::setIsWithinCombatRange(const bool state)
{
	m_isWithinCombatRange = state;
}

bool CloseCombatCondition::Condition()
{
	return m_isWithinCombatRange;
}