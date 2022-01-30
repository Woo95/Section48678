#include "SpaceShip.h"
#include "TextureManager.h"
#include "Util.h"
#include "Game.h"

SpaceShip::SpaceShip()
{
	TextureManager::Instance().load("../Assets/textures/ncl.png", "space_ship");

	const auto size = TextureManager::Instance().getTextureSize("space_ship");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(100.0f, 400.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->acceleration = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	// starting motion properties
	m_maxSpeed = 5.0f;	// a max number of pixels moved per frame
	m_turnRate = 5.0f;	// a max number of degrees to turn each time-step
	m_accelerationRate = 2.0f;	// a max number of pixels to add to the velocity each frame

	setType(AGENT);
}

SpaceShip::~SpaceShip()
= default;

void SpaceShip::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the space_ship
	TextureManager::Instance().draw("space_ship", x, y, 0, 255, isCentered());
}

void SpaceShip::update()
{
	m_move();
}

void SpaceShip::clean()
{

}

float SpaceShip::getMaxSpeed() const
{
	return m_maxSpeed;
}

float SpaceShip::getTurnRate() const
{
	return m_turnRate;
}

float SpaceShip::getAccelerationRate() const
{
	return m_accelerationRate;
}

glm::vec2 SpaceShip::getDesiredVelocity() const
{
	return m_desiredVelocity;
}

void SpaceShip::setMaxSpeed(const float speed)
{
	m_maxSpeed = speed;
}

void SpaceShip::setTurnRate(const float angle)
{
	m_turnRate = angle;
}

void SpaceShip::setAccelerationRate(const float rate)
{
	m_accelerationRate = rate;
}

void SpaceShip::setDesiredVelocity(const glm::vec2 target_position)
{
	m_desiredVelocity = Util::normalize(target_position - getTransform()->position);
}

void SpaceShip::Seek()
{

}

void SpaceShip::LookWhereYoureGoing()
{

}

void SpaceShip::m_move()
{
	//								 new Position	 position term	  velocity term			   acceleration
	// kinematic equation for motion --> Pf =			Pi	+			Vi*(time) +			(0.5)*Ai*(time * time)

	const float dt = TheGame::Instance().getDeltaTime();

	// compute the position term
	const glm::vec2 initial_position = getTransform()->position;

	auto velocity_plus_acceleration = getRigidBody()->velocity + getRigidBody()->acceleration;

	// compute the velocity term
	const glm::vec2 velocity_term = getRigidBody()->velocity *dt;

	// compute the acceleration term
	const glm::vec2 acceleration_term = getRigidBody()->acceleration * 0.5f * dt;

	// compute the new position
	glm::vec2 final_position = initial_position + velocity_term + acceleration_term;

	getTransform()->position = final_position * getMaxSpeed();

	getRigidBody()->velocity += getRigidBody()->acceleration;
}








