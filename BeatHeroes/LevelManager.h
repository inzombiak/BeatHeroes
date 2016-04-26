#pragma once

#include "EnemyManager.h"
#include "IHero.h"

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

	std::vector<CellData> GetDrawingData() const
	{
		return m_drawData;
	}

	void RotateHero(float angle) const
	{
		m_hero->Rotate(angle);
	}

	std::pair<int, int> MoveHero(float direction) const
	{
		auto result = m_hero->GetPos();

		auto newPos = m_hero->Move(direction);

		if (newPos.first < 0 || newPos.first >= m_columns || newPos.second <= 0 || newPos.second > m_rows)
			m_hero->SetPos(result);
		else
			result = newPos;

		return result;
	}

	void Clear();

	IHero* GetHero()
	{
		return m_hero;
	}

private:
	void ProcessUpdate();

	int m_beatPause = 0;
	int m_beatBuffer = 0;
	int m_rows = -1, m_columns = -1;

	std::string m_levelFilepath;
	IHero* m_hero;
	EnemyManager m_enemyManager;
	AbilityInfo m_playerAttackInfo;
	std::vector<CellData> m_drawData;
};