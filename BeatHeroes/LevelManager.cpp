#include "LevelManager.h"
#include "IHero.h"

#include "LuaWrapper\LUAWrapper.h"

LevelManager::LevelManager()
{
	m_hero = 0;
}


LevelManager::~LevelManager()
{
	Clear();
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

	m_beatPause = LuaWrapper::GetInstance().RunFunction<int>("Level", "GetBeatPause", "thisLevel");
	m_beatBuffer = LuaWrapper::GetInstance().RunFunction<int>("Level", "GetBeatBuffer", "thisLevel");
	m_tmxPath = LuaWrapper::GetInstance().RunFunction<std::string>("Level", "GetTmxPath", "thisLevel");

	LoadTmx();
}

void LevelManager::Update()
{
	m_enemyManager.Update();
	m_hero->Update();

	ProcessUpdate();
}

void LevelManager::Draw(sf::RenderWindow& rw)
{
	for (int i = 0; i < m_rows; ++i)
		for (int j = 0; j < m_columns; ++j)
			m_gridItems[i][j]->Draw(rw);

	auto drawData = GetDrawingData();
	for (int i = 0; i < drawData.size(); ++i)
	{
		if (drawData[i].row < 0 || drawData[i].row >= m_rows || drawData[i].column < 0 || drawData[i].column >= m_columns)
			continue;

		m_gridItems[drawData[i].row][drawData[i].column]->DrawCell(sf::Color(drawData[i].R, drawData[i].G, drawData[i].B), rw);
	}
}



void LevelManager::LoadTmx()
{
	tinyxml2::XMLError error = m_xmlDoc.LoadFile(m_tmxPath.c_str());

	tinyxml2::XMLNode* pRoot = m_xmlDoc.FirstChildElement("map");
	tinyxml2::XMLElement* pMapParam = pRoot->ToElement();

	pMapParam->QueryAttribute("width", &m_columns);
	pMapParam->QueryAttribute("height", &m_rows);
	pMapParam->QueryAttribute("tileheight", &m_tileHeight);
	pMapParam->QueryAttribute("tilewidth", &m_tileWidth);

	pMapParam = pRoot->FirstChildElement("tileset");
	tinyxml2::XMLElement* pTilesetParam = pMapParam->FirstChildElement("image");
	std::string tileSetPath = pTilesetParam->Attribute("source");
	pTilesetParam->QueryAttribute("width", &m_textureWidth);
	pTilesetParam->QueryAttribute("height", &m_textureHeight);

	tileSetPath.erase(0, 3);

	m_levelTileSet.loadFromFile(tileSetPath);

	tinyxml2::XMLElement* pLayer = pRoot->FirstChildElement("layer");
	tinyxml2::XMLElement* pData = pLayer->FirstChildElement("data");
	tinyxml2::XMLElement* pTile = pData->FirstChildElement("tile");

	int worldIndex = 0;
	int tileIndex;
	m_gridItems.resize(m_rows);
	GridItem* temp;
	sf::Sprite gridSprite;
	double x, y;
	for (int i = 0; i < m_rows; ++i)
	{
		m_gridItems.push_back(std::vector<GridItem*>());
		m_gridItems[i].resize(m_columns);
		y = i*m_tileHeight;
		for (int j = 0; j < m_columns; ++j)
		{
			pTile->QueryAttribute("gid", &tileIndex);
			worldIndex++;
			x = j * m_tileWidth;
			gridSprite = CreateGridSprite(tileIndex, worldIndex);
			temp = new GridItem(x, y, m_tileWidth, m_tileHeight, gridSprite);
			if (i == m_hero->GetPos().second && j == m_hero->GetPos().first)
			{
				temp->AddHero(m_hero);
				temp->SetColor(sf::Color::Green);
			}

			m_gridItems[i][j] = temp;

			pTile = pTile->NextSiblingElement("tile");
		}
	}

}

sf::Sprite LevelManager::CreateGridSprite(int tileIndex, int worldIndex)
{
	sf::Sprite sprite;
	
	int layer;

	int textureI = std::floor((tileIndex - 1) / (m_textureWidth / m_tileWidth));
	int textureJ = (tileIndex - 1) % (m_textureHeight / m_tileHeight);

	sf::IntRect subRect;
	int ty = textureJ*m_tileHeight;
	int tx = textureI*m_tileWidth;
	subRect.top = tx;
	subRect.left = ty;
	subRect.width = m_tileWidth;
	subRect.height = m_tileHeight;

	sprite.setTexture(m_levelTileSet);
	sprite.setTextureRect(subRect);
	return sprite;
}

void LevelManager::Clear()
{
	for (int i = 0; i < m_gridItems.size(); ++i)
	{
		for (int j = 0; j < m_gridItems[i].size(); ++j)
		{
			delete m_gridItems[i][j];
			m_gridItems[i][j] = 0;
		}

		m_gridItems[i].clear();
	}

	m_gridItems.clear();

	delete m_hero;
	m_hero = 0;
	m_enemyManager.Clear();
	LuaWrapper::GetInstance().Clear();
}

void LevelManager::ProcessUpdate()
{
	for (int i = 0; i < m_rows; ++i)
		for (int j = 0; j < m_columns; ++j)
			m_gridItems[i][j]->Update();

	m_drawData.clear();

	CellData cellD;
	auto enemyD = m_enemyManager.GetEnemyData();
	auto heroPos = m_hero->GetPos();


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

void LevelManager::UseAbility(bool isTap, float angle)
{
	if (isTap)
		m_playerAttackInfo = m_hero->Tap();
	else
		m_playerAttackInfo = m_hero->DoubleTap();
}

void LevelManager::RotateHero(float angle) const
{
	m_hero->Rotate(angle);
}

void LevelManager::MoveHero(float direction) const
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