#include "Obstacle.h"


#include "SoundManager.h"
#include "TextureManager.h"

Obstacle::Obstacle()
{
	TextureManager::Instance().load("../Assets/textures/mine.png", "obstacle");

	auto size = TextureManager::Instance().getTextureSize("obstacle");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(300.0f, 300.0f);

	setType(OBSTACLE);
	getRigidBody()->isColliding = false;

	SoundManager::Instance().load("../Assets/audio/yay.ogg", "yay", SOUND_SFX);
}

Obstacle::~Obstacle()
= default;

void Obstacle::draw()
{
	TextureManager::Instance().draw("obstacle",
		getTransform()->position.x, getTransform()->position.y, 0, 255, true);
}

void Obstacle::update()
{
}

void Obstacle::clean()
{
}

glm::vec2 Obstacle::getGridPosition() const
{
	return m_gridPosition;
}

void Obstacle::setGridPosition(float col, float row)
{
	m_gridPosition = glm::vec2(col, row);
}
