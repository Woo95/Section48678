#include "Tile.h"
#include "Config.h"
#include "Util.h"
#include <sstream>
#include <iomanip>

Tile::Tile() : m_cost(0.0f)
{
	setWidth(Config::TILE_SIZE);
	setHeight(Config::TILE_SIZE);
}

Tile::~Tile() = default;

void Tile::draw()
{
	if (m_status == START)
		Util::DrawFilledRect(getTransform()->position, getWidth(), getHeight(), glm::vec4(0.5f, 1.0f, 0.5f, 1.0f));
	else if (m_status == GOAL)
		Util::DrawFilledRect(getTransform()->position, getWidth(), getHeight(), glm::vec4(1.0f, 0.5f, 0.5f, 1.0f));
	else
		Util::DrawRect(getTransform()->position, getWidth(), getHeight());
}

void Tile::update(){}

void Tile::clean(){}

Tile* Tile::getNeighbourTile(NeighbourTile position)
{
	return m_neighbours[position];
}

void Tile::setNeighbourTile(NeighbourTile position, Tile* tile)
{
	m_neighbours[position] = tile;
}

float Tile::getTileCost() const
{
	return m_cost;
}

void Tile::setTileCost(float cost)
{
	m_cost = cost;

	// format string to 1 decimal place
	std::stringstream stream;
	stream << std::fixed << std::setprecision(1) << m_cost;
	const std::string cost_string = stream.str();

	m_costLabel->setText(cost_string);
}

TileStatus Tile::getTileStatus() const
{
	return m_status;
}

void Tile::setTileStatus(TileStatus status)
{
	m_status = status;

	switch (m_status)
	{
	case UNVISITED:
		m_statusLabel->setText("=");
		break;
	case OPEN:
		m_statusLabel->setText("O");
		break;
	case CLOSED:
		m_statusLabel->setText("C");
		break;
	case IMPASSABLE:
		m_statusLabel->setText("I");
		break;
	case GOAL:
		m_statusLabel->setText("G");
		break;
	case START:
		m_statusLabel->setText("S");
		break;
	}
}

void Tile::addLabels()
{
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	// cost label
	m_costLabel = new Label("99.9", "Consolas", 12);
	m_costLabel->getTransform()->position = getTransform()->position + offset + glm::vec2(0.0f, -6.0f);
	getParent()->addChild(m_costLabel);
	m_costLabel->setEnabled(false);

	// status label
	m_statusLabel = new Label("=", "Consolas", 12);
	m_statusLabel->getTransform()->position = getTransform()->position + offset + glm::vec2(0.0f, 6.0f);
	getParent()->addChild(m_statusLabel);
	m_statusLabel->setEnabled(false);
}

void Tile::setLabelsEnabled(bool state)
{
	m_costLabel->setEnabled(state);
	m_statusLabel->setEnabled(state);
}