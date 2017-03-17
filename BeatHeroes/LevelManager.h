#pragma once

#include "TinyXML2/tinyxml2.h"
#include <string>
#include <vector>
#include "EnemyManager.h"
#include "GridItem.h"
#include "IHero.h"

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
    int m_tempOffsetX = 0;
    int m_tempOffsetY = 0;
    
	LevelManager();
	~LevelManager();
	
	void LoadLevel(const std::string& level);
	void Update();
	void UpdateRender();
	void Draw(sf::RenderWindow& rw);
	void Clear();

	void UseAbility(bool isTap, double angle, bool useSingleScreenControls);
	void RotateHero(double angle) const;
	void MoveHero(double direction) const;
	const std::pair<int, int>& GetHeroPos() const
	{
		return m_hero->GetPos();
	}
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
//	const std::pair<int, int>& GetHeroPos() const
//	{
//		return m_hero->GetPos();
//	}
//	IHero* GetHero()
//	{
//		return m_hero;
//	}
	std::vector<CellData> GetDrawingData() const
	{
		return m_drawData;
	}
//	AbilityInfo GetAttackPattern(bool isTap, float angle = 0) const
//	{
//		if (isTap)
//			return m_hero->Tap();
//
//		return m_hero->DoubleTap();
//	}

private:
    void ProcessUpdate();
    void LoadTmx();
    void CreateCollisionBodies(tinyxml2::XMLElement* collisionRoot);
	sf::Sprite CreateGridSprite(int tileIndex, int worldIndex);

	tinyxml2::XMLDocument m_xmlDoc;
    IHero* m_hero;
    EnemyManager m_enemyManager;
    
    int m_textureWidth = -1, m_textureHeight = -1;
	sf::Texture m_levelTileSet;
	std::string m_tmxPath;
	double m_beatPause = 0;
	double m_beatBuffer = 0;
	int m_rows = -1, m_columns = -1;
	int m_tileHeight = 32, m_tileWidth = 32;

	std::vector<std::vector<GridItem*>> m_gridItems;
	std::string m_levelFilepath;
	AbilityInfo m_playerAttackInfo;
	std::vector<CellData> m_drawData;

    std::vector<std::vector<bool>> m_collisionBodies;
};