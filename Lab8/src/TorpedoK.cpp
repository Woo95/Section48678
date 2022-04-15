#include "TorpedoK.h"
#include "TextureManager.h"

TorpedoK::TorpedoK(float speed, glm::vec2 direction)
	: m_currentAnimationState(FIRED)
{
	TextureManager::Instance().loadSpriteSheet(
		"../Assets/sprites/torpedo.txt",
		"../Assets/sprites/torped_k.png", 
		"torpedoksheet");

	setSpriteSheet(TextureManager::Instance().getSpriteSheet("torpedoksheet"));
	
	// set frame width
	//setWidth(64);

	// set frame height
	//setHeight(64);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PROJECTILE);

	m_direction = { direction.x * speed, direction.y * speed };

	m_buildAnimations();
}

TorpedoK::~TorpedoK()
= default;

void TorpedoK::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the player according to animation state
	switch(m_currentAnimationState)
	{
	case FIRED:
		TextureManager::Instance().playAnimation("torpedoksheet", getAnimation("fired"),
			x, y, 5.0f, 0, 255, true);
		break;
	default:
		break;
	}
	
}

void TorpedoK::update()
{
	// For a homing torpedo, we need to calculate a new destination direction each time. 
	this->getTransform()->position += m_direction;
}

void TorpedoK::clean(){}

void TorpedoK::setAnimationState(const TorpedoAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void TorpedoK::m_buildAnimations()
{
	Animation firedAnimation = Animation();

	firedAnimation.name = "fired";
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired1"));
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired2"));
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired3"));
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired4"));
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired5"));
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired6"));
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired7"));
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired8"));

	setAnimation(firedAnimation);
}
