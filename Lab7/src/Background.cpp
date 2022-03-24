#include "Background.h"
#include "SoundManager.h"
#include "TextureManager.h"

Background::Background()
{
	TextureManager::Instance().load("../Assets/textures/nebula.png", "bg");

	auto size = TextureManager::Instance().getTextureSize("bg");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(size.x/2, size.y/2);

	setType(NONE);
	getRigidBody()->isColliding = false;
}

Background::~Background()
= default;

void Background::draw()
{
	TextureManager::Instance().draw("bg",
		getTransform()->position.x, getTransform()->position.y, 0, 255, true);
}

void Background::update()
{
}

void Background::clean()
{
}
