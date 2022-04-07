#include "PlayerDetectedCondition.h"

PlayerDetectedCondition::PlayerDetectedCondition(bool detected)
{
	setDetected(detected);
	m_name = "Enemy Health Condition";
}

PlayerDetectedCondition::~PlayerDetectedCondition()
= default;

bool PlayerDetectedCondition::getDetected()
{
	return m_detected; // This is so the player can always be detected after the first time.
}

void PlayerDetectedCondition::setDetected(bool detected)
{
	m_detected = detected;
}

bool PlayerDetectedCondition::Condition()
{
	return m_detected;
}