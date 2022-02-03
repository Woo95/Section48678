#pragma once
#ifndef __AGENT__
#define __AGENT__

#include <glm/vec4.hpp>
#include "DisplayObject.h"

class Agent : public DisplayObject
{
public:
	Agent();
	~Agent();

	// Inherited via GameObject
	void draw() override = 0;
	void update() override = 0;
	void clean() override = 0;

	// getters
	glm::vec2 getTargetPosition() const;
	glm::vec2 getCurrentDirection() const;
	float getLOSDistance() const;
	bool hasLOS() const;
	float getCurrentHeading() const;
	glm::vec4 getLOSColour() const;

	// Added second left and right whiskers
	glm::vec2 getLeftSideLOSEndPoint() const;
	glm::vec2 getRightSideLOSEndPoint() const;

	glm::vec2 getLeftLOSEndPoint() const;
	glm::vec2 getMiddleLOSEndPoint() const;
	glm::vec2 getRightLOSEndPoint() const;
	bool* getCollisionWhiskers(); // Returns entire array.
	glm::vec4 getLineColor(int index);
	float getWhiskerAngle() const;


	// setters
	void setTargetPosition(glm::vec2 new_position);
	void setCurrentDirection(glm::vec2 new_direction);
	void setLOSDistance(float distance);
	void setHasLOS(bool state);
	void setCurrentHeading(float heading);
	void setLOSColour(glm::vec4 colour);

	// Added second left and right whiskers
	void setLeftSideLOSEndPoint(glm::vec2 point);
	void setRightSideLOSEndPoint(glm::vec2 point);

	void setLeftLOSEndPoint(glm::vec2 point);
	void setMiddleLOSEndPoint(glm::vec2 point);
	void setRightLOSEndPoint(glm::vec2 point);
	void setLineColor(int index, glm::vec4 color);
	void setWhiskerAngle(float a);
	void updateWhiskers(float a);

private:
	void m_changeDirection();
	float m_currentHeading; // angle the ship is looking
	glm::vec2 m_currentDirection; // vector the ship is moving
	glm::vec2 m_targetPosition; // vector representing the target.position

	// LOS
	float m_LOSDistance;
	bool m_hasLOS;
	glm::vec4 m_LOSColour;

	// Added second left and right whiskers
	glm::vec2 m_leftSideLOSEndPoint;
	glm::vec2 m_rightSideLOSEndPoint;

	glm::vec2 m_leftLOSEndPoint;
	glm::vec2 m_middleLOSEndPoint;
	glm::vec2 m_rightLOSEndPoint;
	glm::vec4 m_lineColor[5]; // Change to 5 when you add the two extra whiskers
	bool m_collisionWhiskers[5];

	float m_whiskerAngle;
};



#endif /* defined ( __AGENT__) */