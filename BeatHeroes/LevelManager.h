#pragma once

#include "TinyXML2\tinyxml2.h"

#include "EnemyManager.h"
#include "GridItem.h"

struct CellData
{
	int row;
	int column;
	int R, G, B;
};

class IHero;
class LevelManager
{
public:
	LevelManager();
	~LevelManager();
	
	void LoadLevel(const std::string& level);
	void Update();
	void UpdateRender();
	void Draw(sf::RenderWindow& rw);
	void Clear();

	void UseAbility(bool isTap, double angle = 0);
	void RotateHero(double angle) const;
	void MoveHero(double direction) const;

	double GetBeatPause() const
	{
		return m_beatPause;
	}
	double GetBeatBuffer() const
	{
		return m_beatBuffer;
	}
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
	IHero* GetHero()
	{
		return m_hero;
	}
	std::vector<CellData> GetDrawingData() const
	{
		return m_drawData;
	}
	AbilityInfo GetAttackPattern(bool isTap, float angle = 0) const
	{
		if (isTap)
			return m_hero->Tap();

		return m_hero->DoubleTap();
	}

private:
	tinyxml2::XMLDocument m_xmlDoc;
	void LoadTmx();
	sf::Sprite CreateGridSprite(int tileIndex, int worldIndex);
	int m_textureWidth = -1, m_textureHeight = -1;
	sf::Texture m_levelTileSet;

	void ProcessUpdate();
	std::string m_tmxPath;
	double m_beatPause = 0;
	double m_beatBuffer = 0;
	int m_rows = -1, m_columns = -1;
	int m_tileHeight = -1, m_tileWidth = -1;

	std::vector<std::vector<GridItem*>> m_gridItems;
	std::string m_levelFilepath;
	IHero* m_hero;
	EnemyManager m_enemyManager;
	AbilityInfo m_playerAttackInfo;
	std::vector<CellData> m_drawData;
};