#include "LOSCondition.h"

LOSCondition::LOSCondition(bool LOS)
{
	setLOS(LOS);
	m_name = "LOS Condition";
}

LOSCondition::~LOSCondition()= default;

void LOSCondition::setLOS(bool state)
{
	m_hasLOS = state;
	m_name = state;
}

bool LOSCondition::Condition()
{
	return m_hasLOS;
}