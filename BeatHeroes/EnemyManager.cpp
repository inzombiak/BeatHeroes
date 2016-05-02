#include "EnemyManager.h"

#include "TinyXML2\tinyxml2.h"
#include "LuaWrapper\LUAWrapper.h"
#include "LuaWrapper\LUAObject.h"

EnemyManager::EnemyManager()
{
}


EnemyManager::~EnemyManager()
{
}

void EnemyManager::Init(const std::string& envName, const std::string& tableName, const std::string& globalName)
{
	std::vector<std::shared_ptr<LuaObject>> luaPtrs = LuaWrapper::GetInstance().TableToVector<std::shared_ptr<LuaObject>>(envName, tableName, globalName);

	if (luaPtrs.size() < 1)
		return;
	
	LoadSpritesheet(luaPtrs[0]);
	
	for (unsigned int i = 0; i < luaPtrs.size(); ++i)
	{
		m_enemies.push_back(Enemy(i, luaPtrs[i], m_enemySpriteSheet));
	}
}

void EnemyManager::Update()
{
	for (unsigned int i = 0; i < m_enemies.size(); ++i)
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

void EnemyManager::UpdateRender()
{
	for (unsigned int i = 0; i < m_enemies.size(); ++i)
		m_enemies[i].UpdateRender();
}
void EnemyManager::Draw(sf::RenderWindow& rw)
{
	for (unsigned int i = 0; i < m_enemies.size(); ++i)
		m_enemies[i].Draw(rw);
}


std::vector<EnemyData> EnemyManager::GetEnemyData()
{
	std::vector<EnemyData> result;
	result.resize(m_enemies.size());

	for (unsigned int i = 0; i < result.size(); ++i)
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

void EnemyManager::LoadSpritesheet(std::shared_ptr<LuaObject> enemyPtr)
{
	std::string filename = enemyPtr->CallFunction<std::string>("GetAnimationPath");
	std::string ssFP;

	{
		tinyxml2::XMLDocument animFile;
		if (animFile.LoadFile(filename.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
			printf("ERROR Loading %s", filename);

		tinyxml2::XMLElement* pAnimations = animFile.FirstChildElement("animations");
		ssFP = pAnimations->Attribute("spriteSheet");

		if (ssFP.empty())
			return;

	}

	tinyxml2::XMLDocument spriteFile;
	ssFP = ConvertRelativePathToStatic(filename, ssFP);
	if (spriteFile.LoadFile(ssFP.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
		std::cout << "ERROR LOADING: " << ssFP << std::endl;

	std::string animFP = enemyPtr->CallFunction<std::string>("GetAnimationPath");
	tinyxml2::XMLDocument animationFile;
	if (animationFile.LoadFile(animFP.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
		printf("ERROR Loading %s", animFP);

	tinyxml2::XMLElement* pImg = spriteFile.FirstChildElement("img");

	std::string ssFPRaw = pImg->Attribute("name");
	ssFPRaw = ConvertRelativePathToStatic(ssFP, ssFPRaw);
	m_enemySpriteSheet.loadFromFile("Images/DungeonCrawl.png");
}