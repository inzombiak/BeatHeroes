#include "LevelManager.h"
#include "IHero.h"

#include "LuaWrapper/LUAWrapper.h"

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

	m_columns = LuaWrapper::GetInstance().RunFunction<int>("Level", FuncNameInfo("GetColumns", "thisLevel"));
	m_rows = LuaWrapper::GetInstance().RunFunction<int>("Level", FuncNameInfo("GetRows", "thisLevel"));
    
    int heroY = LuaWrapper::GetInstance().RunFunction<int>("Level", FuncNameInfo("GetHeroY", "thisLevel"));
    int heroX = LuaWrapper::GetInstance().RunFunction<int>("Level", FuncNameInfo("GetHeroX", "thisLevel"));
    
    m_enemyManager.m_tempOffsetX = m_tempOffsetX;
    m_enemyManager.m_tempOffsetY = m_tempOffsetY;
	m_enemyManager.Init("Level", "_enemies", "thisLevel");
	

	m_beatPause = LuaWrapper::GetInstance().RunFunction<double>("Level", FuncNameInfo("GetBeatPause", "thisLevel"));
	m_beatBuffer = LuaWrapper::GetInstance().RunFunction<double>("Level", FuncNameInfo("GetBeatBuffer", "thisLevel"));
	m_tmxPath = LuaWrapper::GetInstance().RunFunction<std::string>("Level", FuncNameInfo("GetTmxPath", "thisLevel"));

    LoadTmx();
    
    m_enemyManager.SetWorldBounds(std::pair<int, int>(m_columns, m_rows));
    m_enemyManager.SetLevelCollisionBodies(m_collisionBodies);
    m_hero = new IHero(1, "Scripts/Heroes/Warrior Hero/WarriorHero.lua");
   // m_hero->m_tempOffsetX = m_tempOffsetX;
   // m_hero->m_tempOffsetY = m_tempOffsetY;

    m_hero->SetPos(std::pair<int, int>(heroX, heroY));
}

void LevelManager::UpdateRender()
{
	m_hero->UpdateRender();
	m_enemyManager.UpdateRender();
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
	for (unsigned int i = 0; i < drawData.size(); ++i)
	{
		if (drawData[i].row < 0 || drawData[i].row >= m_rows || drawData[i].column < 0 || drawData[i].column >= m_columns)
			continue;

		m_gridItems[drawData[i].row][drawData[i].column]->DrawCell(sf::Color(drawData[i].R, drawData[i].G, drawData[i].B), rw);
	}

	m_hero->Draw(rw);
	m_enemyManager.Draw(rw);
}



void LevelManager::LoadTmx()
{
	m_xmlDoc.LoadFile(m_tmxPath.c_str());

	tinyxml2::XMLNode* pRoot = m_xmlDoc.FirstChildElement("map");
	tinyxml2::XMLElement* pMapParam = pRoot->ToElement();

	pMapParam->QueryAttribute("width", &m_columns);
	pMapParam->QueryAttribute("height", &m_rows);
	pMapParam->QueryAttribute("tileheight", &m_tileHeight);
	pMapParam->QueryAttribute("tilewidth", &m_tileWidth);
    
    m_collisionBodies.resize(m_columns, std::vector<bool>(m_rows ,false));
    
	pMapParam = pRoot->FirstChildElement("tileset");
	tinyxml2::XMLElement* pTilesetParam = pMapParam->FirstChildElement("image");
	std::string tileSetPath = pTilesetParam->Attribute("source");
	pTilesetParam->QueryAttribute("width", &m_textureWidth);
	pTilesetParam->QueryAttribute("height", &m_textureHeight);

	tileSetPath.erase(0, 3);
	m_levelTileSet.loadFromFile(tileSetPath.c_str());
    
	auto size = m_levelTileSet.getSize();
	auto width = size.x;
    std::cout << width << std::endl;
	tinyxml2::XMLElement* pLayer = pRoot->FirstChildElement("layer");

    
	int worldIndex = 0;
	int tileIndex;
	m_gridItems.resize(m_rows);
	GridItem* temp;
	sf::Sprite gridSprite;
    int zPos = -2;
	int x, y;
    while(pLayer)
    {
        tinyxml2::XMLElement* pData = pLayer->FirstChildElement("data");
        tinyxml2::XMLElement* pTile = pData->FirstChildElement("tile");
        for (int i = 0; i < m_rows; ++i)
        {
            m_gridItems.push_back(std::vector<GridItem*>());
            m_gridItems[i].resize(m_columns);
            y =  m_tempOffsetY + i*m_tileHeight;
            for (int j = 0; j < m_columns; ++j)
            {
                pTile->QueryAttribute("gid", &tileIndex);
                if(tileIndex == 0)
                {
                    pTile = pTile->NextSiblingElement("tile");
                    continue;

                }
                
                worldIndex++;
                x =  m_tempOffsetX + j * m_tileWidth;
                gridSprite = CreateGridSprite(tileIndex, worldIndex);
               
                temp = new GridItem(x, y, m_tileWidth, m_tileHeight, gridSprite);
                m_gridItems[i][j] = temp;
                pTile = pTile->NextSiblingElement("tile");

            }
        }
        zPos++;
        pLayer = pLayer->NextSiblingElement("layer");
    }
    tinyxml2::XMLElement* pCollRoot = pRoot->FirstChildElement("objectgroup");
    std::string collLayerName = "Collisions";
    if(collLayerName.compare(pCollRoot->Attribute("name")) == 0)
        CreateCollisionBodies(pCollRoot);
}

void LevelManager::CreateCollisionBodies(tinyxml2::XMLElement *pCollisionRoot)
{
    tinyxml2::XMLElement* pObj = pCollisionRoot->FirstChildElement("object");
    int column, row, firstRow, x, y, width, height, numTilesSpannedX, numTilesSpannedY;
    sf::RectangleShape spr;
	sf::IntRect rect;
    
    while(pObj)
    {
        pObj->QueryAttribute("x", &x);
        pObj->QueryAttribute("y", &y);
        pObj->QueryAttribute("width", &width);
        pObj->QueryAttribute("height", &height);
        numTilesSpannedX = width/m_tileWidth;
        numTilesSpannedY = height/m_tileHeight;
        std::cout <<"ID: " << pObj->Attribute("id") << std::endl;
        firstRow = row = y/m_tileHeight;
        column = x/m_tileWidth;
        
        for(unsigned int i = 0; i < numTilesSpannedX; ++i)
        {
            row = firstRow;
            for(unsigned int j = 0;  j < numTilesSpannedY; ++j)
            {
                m_collisionBodies[column][row] = true;
                row++;
            }
            
            column++;
        }
        //x = 600;
      //  y = 200;
		rect.left = m_tempOffsetX + x - m_tileHeight / 2;
        rect.top = m_tempOffsetY - y - height + m_tileWidth/2;
        rect.width = width;
        rect.height = height;
        
		spr.setSize(sf::Vector2f(rect.width, rect.height));
		sf::Color color = sf::Color::Magenta;
		color.a = 0.3;
		spr.setFillColor(color);

        
        pObj = pObj->NextSiblingElement();
    }
    
}

sf::Sprite LevelManager::CreateGridSprite(int tileIndex, int worldIndex)
{
	sf::Sprite sprite;
	int textureI = (int)std::floor((tileIndex - 1) / (m_textureWidth / m_tileWidth));
	int textureJ = (int)(tileIndex - 1) % (m_textureHeight / m_tileHeight);

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
	for (unsigned int i = 0; i < m_gridItems.size(); ++i)
	{
		for (unsigned int j = 0; j < m_gridItems[i].size(); ++j)
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
		
		cellD.row = m_playerAttackInfo.pattern[j].second + heroPos.second;
		cellD.column = m_playerAttackInfo.pattern[j].first + heroPos.first;
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
            
			for (unsigned int k = 0; k < enemyD[i].pattern.size(); ++k)
			{
				cellD.row = enemyD[i].pos.second + enemyD[i].pattern[k].second;
				cellD.column = enemyD[i].pos.first + enemyD[i].pattern[k].first;
				cellD.R = 255;
				cellD.G = 0;
				cellD.B = 0;
				m_drawData.push_back(cellD);
			}
		}

//		for (unsigned int j = 0; j < m_playerAttackInfo.pattern.size(); ++j)
//		{
//			if (enemyD[i].pos == m_playerAttackInfo.pattern[j])
//			{
//				m_enemyManager.SetEnemyHP(enemyD[i].index, enemyD[i].hp - m_playerAttackInfo.damage);
//			}
//		}
	}

	m_playerAttackInfo.damage = 0;
}

void LevelManager::UseAbility(bool isTap, double angle, bool sglScreenControls)
{
	if (isTap)
		m_playerAttackInfo = m_hero->Tap(angle, sglScreenControls);
	else
		m_playerAttackInfo = m_hero->DoubleTap(angle, sglScreenControls);
}

void LevelManager::RotateHero(double angle) const
{
	m_hero->Rotate(angle);
}

void LevelManager::MoveHero(double direction) const
{
	auto oldPos = m_hero->GetPos();
	auto newPos = m_hero->Move(direction);

	if (newPos.first < 0 || newPos.first >= m_columns || newPos.second <= 0 || newPos.second > m_rows || m_collisionBodies[newPos.first][newPos.second])
	{
		m_hero->SetPos(oldPos);
		return;
	}

}