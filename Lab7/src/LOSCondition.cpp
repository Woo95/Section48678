#include "LOSCondition.h"

LOSCondition::LOSCondition(bool LOS)
{
	m_name = "LOS Condition";
}

LOSCondition::~LOSCondition()
= default;

bool LOSCondition::Condition()
{
	return getAgent()->hasLOS();
}