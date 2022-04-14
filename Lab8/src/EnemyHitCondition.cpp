#include "EnemyHitCondition.h"

EnemyHitCondition::EnemyHitCondition(bool hit)
{
	setIsHit(hit);
	m_name = "Enemy Hit Condition";
}

EnemyHitCondition::~EnemyHitCondition()
= default;

void EnemyHitCondition::setIsHit(bool hit)
{
	m_isHit = hit;
}

bool EnemyHitCondition::Condition()
{
	return m_isHit;
}