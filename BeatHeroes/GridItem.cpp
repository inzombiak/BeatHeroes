#include "GridItem.h"
#include <SFML\Window.hpp>

GridItem::GridItem(int x, int y, int w, int h, sf::Sprite sprite) : m_sprite(sprite)
{
	m_cell.setSize(sf::Vector2f(w, h));
	m_cell.setPosition(x, y);
	m_cell.setFillColor(sf::Color::White);

	m_directionRect.setSize(sf::Vector2f(w / 5, h / 5));
	m_directionRect.setFillColor(sf::Color::Magenta);
	m_sprite.setPosition(x, y);
}

GridItem::~GridItem()
{

}

void GridItem::Draw(sf::RenderWindow& rw) 
{
	rw.draw(m_sprite);
	if (m_hero)
	{
		rw.draw(m_cell);
		int xPos = m_cell.getPosition().x + (m_cell.getSize().x - m_directionRect.getSize().x) * (1 + std::cos(m_hero->GetDirection())) / 2;
		int yPos = m_cell.getPosition().y + (m_cell.getSize().y - m_directionRect.getSize().y) * (1 + std::sin(m_hero->GetDirection())) / 2;
		m_directionRect.setPosition(xPos, yPos);

		rw.draw(m_directionRect);
	}

}

void GridItem::DrawCell(sf::Color color, sf::RenderWindow& rw)
{
	SetColor(color);
	rw.draw(m_cell);
}
