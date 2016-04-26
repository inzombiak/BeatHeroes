#pragma once

#include "TinyXML2\tinyxml2.h"

#include "EnemyManager.h"
#include "IHero.h"
#include "GridItem.h"

struct CellData
{
	int row;
	int column;
	int R, G, B;
};


class LevelManager
{
public:
	LevelManager();
	~LevelManager();
	
	void LoadLevel(const std::string& level);

	int GetRows() const
	{
		return m_rows;
	}
	int GetColumns() const
	{
		return m_columns;
	}
	const std::pair<int, int>& GetHeroPos() const
	{
		return m_hero->GetPos();
	}
	AbilityInfo GetAttackPattern(bool isTap, float angle = 0) const
	{
		if (isTap)
			return m_hero->Tap();
		
		return m_hero->DoubleTap();
	}

	void UseAbility(bool isTap, float angle = 0)
	{
		if (isTap)
			m_playerAttackInfo = m_hero->Tap();
		else
			m_playerAttackInfo = m_hero->DoubleTap();
	}

	int GetBeatPause() const
	{
		return m_beatPause;
	}
	int GetBeatBuffer() const
	{
		return m_beatBuffer;
	}

	void Update()
	{
		m_enemyManager.Update();
		m_hero->Update();

		ProcessUpdate();
	}
	
	void Draw(sf::RenderWindow& rw);

	std::vector<CellData> GetDrawingData() const
	{
		return m_drawData;
	}

	void RotateHero(float angle) const
	{
		m_hero->Rotate(angle);
	}
	
	void MoveHero(float direction) const
	{
		auto oldPos = m_hero->GetPos();
		auto newPos = m_hero->Move(direction);

		if (newPos.first < 0 || newPos.first >= m_columns || newPos.second <= 0 || newPos.second > m_rows)
		{
			m_hero->SetPos(oldPos);
			return;
		}

		m_gridItems[oldPos.second][oldPos.first]->RemoveHero();
		m_gridItems[oldPos.second][oldPos.first]->SetColor(sf::Color::White);

		m_gridItems[newPos.second][newPos.first]->AddHero(m_hero);
		m_gridItems[newPos.second][newPos.first]->SetColor(sf::Color::Green);

	}

	void Clear();

	IHero* GetHero()
	{
		return m_hero;
	}

private:
	tinyxml2::XMLDocument m_xmlDoc;
	void LoadTmx();
	sf::Sprite CreateGridSprite(int tileIndex, int worldIndex);
	int m_textureWidth = -1, m_textureHeight = -1;
	sf::Texture m_levelTileSet;

	void ProcessUpdate();
	std::string m_tmxPath;
	int m_beatPause = 0;
	int m_beatBuffer = 0;
	int m_rows = -1, m_columns = -1;
	int m_tileHeight = -1, m_tileWidth = -1;

	std::vector<std::vector<GridItem*>> m_gridItems;
	std::string m_levelFilepath;
	IHero* m_hero;
	EnemyManager m_enemyManager;
	AbilityInfo m_playerAttackInfo;
	std::vector<CellData> m_drawData;
};