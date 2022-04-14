#pragma once
#ifndef __PLAYER_DETECTED_CONDITION__
#define __PLAYER_DETECTED_CONDITION__
#include "ConditionNode.h"

class PlayerDetectedCondition : public ConditionNode
{
public:
	PlayerDetectedCondition(bool detected = false);
	virtual ~PlayerDetectedCondition();

	// Getters and Setters
	bool getDetected();
	void setDetected(bool detected);

	virtual bool Condition() override;
private:
	bool m_detected;
};

#endif