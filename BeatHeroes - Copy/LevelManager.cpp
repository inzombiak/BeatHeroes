#include "LevelManager.h"

#include "LuaWrapper\LUAWrapper.h"

LevelManager::LevelManager()
{
	m_hero = 0;
}


LevelManager::~LevelManager()
{
	if (m_hero)
	{
		delete m_hero;
		m_hero = 0;
	}
}

void LevelManager::LoadLevel(const std::string& level)
{
	if (m_hero)
	{
		delete m_hero;
		m_hero = 0;
	}

	LuaWrapper::GetInstance().LoadScript("Level", level);

	m_columns = LuaWrapper::GetInstance().RunFunction<int>("Level", "GetColumns", "thisLevel");
	m_rows = LuaWrapper::GetInstance().RunFunction<int>("Level", "GetRows", "thisLevel");

	m_hero = new IHero(1, "Scripts/Heroes/Warrior Hero/WarriorHero.lua");

	int heroY = LuaWrapper::GetInstance().RunFunction<int>("Level", "GetHeroY", "thisLevel");
	int heroX = LuaWrapper::GetInstance().RunFunction<int>("Level", "GetHeroX", "thisLevel");
	
	m_hero->SetPos(std::pair<int, int>(heroX, heroY));
	m_enemyManager.Init("Level", "_enemies", "thisLevel");
	m_enemyManager.SetWorldBounds(std::pair<int, int>(m_columns, m_rows));
}

void LevelManager::Clear()
{
	delete m_hero;
	m_hero = 0;
	m_enemyManager.Clear();
	LuaWrapper::GetInstance().Clear();
}

void LevelManager::ProcessUpdate()
{
	m_drawData.clear();

	CellData cellD;
	auto enemyD = m_enemyManager.GetEnemyData();
	auto heroPos = m_hero->GetPos();
	cellD.row = heroPos.second; 
	cellD.column = heroPos.first;
	cellD.R = 0;
	cellD.G = 0;
	cellD.B = 255;
	m_drawData.push_back(cellD);

	for (unsigned int j = 0; j < m_playerAttackInfo.pattern.size(); ++j)
	{
		m_playerAttackInfo.pattern[j].first += heroPos.first;
		m_playerAttackInfo.pattern[j].second += heroPos.second;
		
		cellD.row = m_playerAttackInfo.pattern[j].second;
		cellD.column = m_playerAttackInfo.pattern[j].first;
		cellD.R = 0;
		cellD.G = 255;
		cellD.B = 0;
		m_drawData.push_back(cellD);
	}

	for (unsigned int i = 0; i < enemyD.size(); ++i)
	{
		cellD.row = enemyD[i].pos.second;
		cellD.column = enemyD[i].pos.first;
		cellD.R = 255;
		cellD.G = 255;
		cellD.B = 0;
		m_drawData.push_back(cellD);

		for (unsigned int k = 0; k < enemyD[i].pattern.size(); ++k)
		{
			cellD.row = enemyD[i].pos.second + enemyD[i].pattern[k].second;
			cellD.column = enemyD[i].pos.first + enemyD[i].pattern[k].first;
			cellD.R = 255;
			cellD.G = 0;
			cellD.B = 0;
			m_drawData.push_back(cellD);
		}

		for (unsigned int j = 0; j < m_playerAttackInfo.pattern.size(); ++j)
		{
			if (enemyD[i].pos == m_playerAttackInfo.pattern[j])
			{
				m_enemyManager.SetEnemyHP(enemyD[i].index, enemyD[i].hp - m_playerAttackInfo.damage);
			}
		}
	}


	m_playerAttackInfo.damage = 0;
	m_playerAttackInfo.pattern.clear();
}