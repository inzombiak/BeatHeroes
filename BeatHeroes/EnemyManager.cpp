#include "EnemyManager.h"

#include "TinyXML2/tinyxml2.h"
#include "LuaWrapper/LUAWrapper.h"
#include "LuaWrapper/LUAObject.h"

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
        m_enemies[i].m_tempOffsetX = m_tempOffsetX;
        m_enemies[i].m_tempOffsetY = m_tempOffsetY;
        m_enemies[i].Init();
	}
}

void EnemyManager::SetLevelCollisionBodies(const std::vector<std::vector<bool>>& collisionBodies)
{
    m_collisionBodies = collisionBodies;
}

void EnemyManager::Update()
{
    bool contact;
    std::pair<int, int> oldPos, pos;
	for (unsigned int i = 0; i < m_enemies.size(); ++i)
	{
        contact = false;
        
		if (!m_enemies[i].IsAlive())
			continue;
        oldPos = m_enemies[i].GetPos();
		m_enemies[i].Update();

		pos = m_enemies[i].GetPos();

		if (pos.first < 0)
            contact = true;
		else if (pos.first >= m_bounds.first)
            contact = true;

		if (pos.second < 0)
            contact = true;
		else if(pos.second >= m_bounds.second)
            contact = true;
		
        if(contact)
        {
            m_enemies[i].SetPos(oldPos.first, oldPos.second);
            m_enemies[i].ContactWithStatic();
            return;
        }
        
        if(m_collisionBodies[pos.first][pos.second])
            contact = true;
        
        if(contact)
        {
            m_enemies[i].SetPos(oldPos.first, oldPos.second);
            m_enemies[i].ContactWithStatic();
            true;
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
	LuaWrapper::GetInstance().RunFunction<void>("Level", FuncNameInfo("KillEnemy", "thisLevel"), index + 1);
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
            std::cout<< "ERROR Loading %s" << filename << std::endl;

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
		std::cout<< "ERROR Loading %s" << animFP << std::endl;

	tinyxml2::XMLElement* pImg = spriteFile.FirstChildElement("img");

	std::string ssFPRaw = pImg->Attribute("name");
	ssFPRaw = ConvertRelativePathToStatic(ssFP, ssFPRaw);

	m_enemySpriteSheet.loadFromFile(ssFPRaw.c_str());
    
}
