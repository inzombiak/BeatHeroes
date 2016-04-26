#include "EnemyManager.h"
#include "LuaWrapper\LUAWrapper.h"

EnemyManager::EnemyManager()
{
}


EnemyManager::~EnemyManager()
{
}

void EnemyManager::Init(const std::string& envName, const std::string& tableName, const std::string& globalName)
{
	std::vector<std::shared_ptr<LuaObject>> luaPtrs = LuaWrapper::GetInstance().TableToVector<std::shared_ptr<LuaObject>>(envName, tableName, globalName);

	for (int i = 0; i < luaPtrs.size(); ++i)
	{
		m_enemies.push_back(Enemy(i, luaPtrs[i]));
	}
}

void EnemyManager::Update()
{
	for (int i = 0; i < m_enemies.size(); ++i)
	{
		if (!m_enemies[i].IsAlive())
			continue;

		m_enemies[i].Update();

		auto pos = m_enemies[i].GetPos();

		if (pos.first < 0)
		{
			m_enemies[i].SetPos(0,pos.second);
			m_enemies[i].ContactWithStatic();
		}
		else if (pos.first > m_bounds.first)
		{
			m_enemies[i].SetPos(m_bounds.first - 1, pos.second);
			m_enemies[i].ContactWithStatic();
		}

		if (pos.second < 0)
		{
			m_enemies[i].SetPos(pos.first,0);
			m_enemies[i].ContactWithStatic();
		}
		else if(pos.second > m_bounds.second)
		{
			m_enemies[i].SetPos(pos.first, m_bounds.second - 1);
			m_enemies[i].ContactWithStatic();
		}
			
	}
}

std::vector<EnemyData> EnemyManager::GetEnemyData()
{
	std::vector<EnemyData> result;
	result.resize(m_enemies.size());

	for (int i = 0; i < result.size(); ++i)
	{
		if (!m_enemies[i].IsAlive())
			continue;

		result[i].pos = m_enemies[i].GetPos();
		result[i].pattern = m_enemies[i].GetPattern();
		result[i].hp = m_enemies[i].GetHealth();
		result[i].dmg = m_enemies[i].GetDamage();
		result[i].index = i;
	}

	return result;
}

void EnemyManager::SetEnemyHP(unsigned int index, int amount)
{
	if (index > m_enemies.size())
		return;

	if (amount <= 0)
		KillEnemy(index);
	else
		m_enemies[index].SetHealth(amount);
}

void EnemyManager::KillEnemy(unsigned int index)
{
	if (index > m_enemies.size())
		return;

	m_unusedEnemyIndicies.push(index);
	LuaWrapper::GetInstance().RunFunction<void>("Level", "KillEnemy", "thisLevel", index + 1);
	LuaWrapper::GetInstance().DeleteLuaObject("Level", m_enemies[index].GetName());

	m_enemies[index].Kill();
}

