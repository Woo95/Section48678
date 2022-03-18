#include "PathNode.h"
#include "Config.h"
#include "Util.h"

PathNode::PathNode()
{
	setWidth(10);
	setHeight(10);
	getTransform()->position = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PATH_NODE);

	setCurrentHeading(0.0f); // current facing angle
	setCurrentDirection(glm::vec2(1.0f, 0.0f)); // vector pointing right

	setLOSDistance(1000.f);
	setLOSColour(glm::vec4(0, 0.5, 1, 1));
}

PathNode::~PathNode() = default;

void PathNode::draw()
{
	// draw node
	if (hasLOS())
	{
		Util::DrawFilledRect(getTransform()->position - glm::vec2(5, 5), getWidth(), getHeight(), 
			glm::vec4(0.0f, 0.75f, 0.0f, 1.0f));
	}
	else
		Util::DrawRect(getTransform()->position - glm::vec2(5, 5), getWidth(), getHeight(), getLOSColour());
}

void PathNode::update()
{
}

void PathNode::clean()
{
}
