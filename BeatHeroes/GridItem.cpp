#include "GridItem.h"
#include <SFML\Window.hpp>

GridItem::GridItem(int x, int y, int w, int h)
{
	m_cell.setSize(sf::Vector2f(w, h));
	m_cell.setPosition(x, y);
	m_cell.setFillColor(sf::Color::White);
	m_cell.setOutlineThickness(1);
	m_cell.setOutlineColor(sf::Color::Black);
	m_directionRect.setSize(sf::Vector2f(w / 5, h / 5));
	m_directionRect.setFillColor(sf::Color::Magenta);
}

GridItem::~GridItem()
{

}

void GridItem::Draw(sf::RenderWindow& rw) 
{
	rw.draw(m_cell);
	if (m_hero)
	{
		int xPos = m_cell.getPosition().x + (m_cell.getSize().x - m_directionRect.getSize().x) * (1 + std::cos(m_hero->GetDirection())) / 2;
		int yPos = m_cell.getPosition().y + (m_cell.getSize().y - m_directionRect.getSize().y) * (1 + std::sin(m_hero->GetDirection())) / 2;
		m_directionRect.setPosition(xPos, yPos);

		rw.draw(m_directionRect);
	}
}