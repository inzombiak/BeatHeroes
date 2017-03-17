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

class LuaObject;
class EnemyManager
{
public:
    int m_tempOffsetX = 0, m_tempOffsetY = 0;
    
    
	EnemyManager();
	~EnemyManager();

    void Init(const std::string& envName, const std::string& tableName,const std::string& globalName = "");
	void Update();
	void UpdateRender();
	void Draw(sf::RenderWindow& rw);
    void SetLevelCollisionBodies(const std::vector<std::vector<bool>>& collisionBodies);
	std::vector<EnemyData> GetEnemyData();

	void Clear()
	{
		m_enemies.clear();
        m_collisionBodies.clear();
	}

	void SetWorldBounds(std::pair<int, int> bounds)
	{
		m_bounds = bounds;
	}


	void SetEnemyHP(unsigned int index, int amount);

private:
	void LoadSpritesheet(std::shared_ptr<LuaObject> enemyPtr);

	std::pair<int, int> m_bounds;
	void KillEnemy(unsigned int index);
	std::vector<Enemy> m_enemies;
	std::queue<unsigned int> m_unusedEnemyIndicies;
    std::vector<std::vector<bool>> m_collisionBodies;
	sf::Texture m_enemySpriteSheet;
};
