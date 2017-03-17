#ifndef GRIDITEM_H
#define GRIDITEM_H

//#include "IHero.h"

#include <SFML/Graphics.hpp>

class GridItem
{
public:
	GridItem(int x, int y, int w, int h, sf::Sprite sprite);
	~GridItem();

	void Update()
	{
	//	if (!m_hero)
		//	m_cell.setFillColor(sf::Color::Transparent);
	}
	void SetColor(sf::Color newColor)
	{
		m_cell.setOutlineColor(newColor);
	}
	void DrawCell(sf::Color color, sf::RenderWindow& rw);
	void Draw(sf::RenderWindow& rw);
	//void AddHero(IHero* hero)
	//{
		//m_hero = hero;
	//}
	//bool HasHero() const
	//{
	//	return m_hero != 0;
	//}
	//void RemoveHero()
	//{
	//	m_hero = 0;
	//}

private:

	sf::Sprite m_sprite;
	sf::RectangleShape m_spriteSheet;
	sf::RectangleShape m_cell;
};

#endif
