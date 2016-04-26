#pragma once
#include "Enemy.h"

#include <vector>
#include <queue>

struct EnemyData
{
	std::pair<int, int> pos;
	std::vector<std::pair<int, int>> pattern;
	int hp;
	int dmg;
	unsigned int index;
};

class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();

	void Init(const std::string& envName, const std::string& tableName, const std::string& globalName = "");
	void Update();
	std::vector<EnemyData> GetEnemyData();

	void Clear()
	{
		m_enemies.clear();
	}

	void SetWorldBounds(std::pair<int, int> bounds)
	{
		m_bounds = bounds;
	}


	void SetEnemyHP(unsigned int index, int amount);

private:
	std::pair<int, int> m_bounds;
	void KillEnemy(unsigned int index);
	std::vector<Enemy> m_enemies;
	std::queue<unsigned int> m_unusedEnemyIndicies;
};

