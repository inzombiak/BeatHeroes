#ifndef GRIDITEM_H
#define GRIDITEM_H

#include <SFML/Graphics.hpp>
#include "IHero.h"

class GridItem
{
public:
	GridItem(int x, int y, int w, int h, sf::Sprite sprite);
	~GridItem();

	void Update()
	{
		if (!m_hero)
			m_cell.setFillColor(sf::Color::White);
	}

	void Draw(sf::RenderWindow& rw);
	void DrawCell(sf::Color color, sf::RenderWindow& rw);
	void AddHero(IHero* hero)
	{
		m_hero = hero;
	}
	bool HasHero() const
	{
		return m_hero != 0;
	}
	void RemoveHero()
	{
		m_hero = 0;
	}
	void SetColor(sf::Color newColor)
	{
		m_cell.setFillColor(newColor);
	}

private:
	sf::Vector2i m_click;

	IHero* m_hero = 0;
	sf::RectangleShape m_directionRect;
	sf::RectangleShape m_cell;
	sf::Sprite m_sprite;
};

#endif
