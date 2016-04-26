#define _USE_MATH_DEFINES
#include <cmath>

#include "Game.h"
#include "GridItem.h"
#include "LuaWrapper\LuaWrapper.h"

void Game::Start()
{
	m_gameState = GameState::Initializing;
	m_renderWindow.create(sf::VideoMode(m_windowWidth, m_windowHeight, 32), "SFML works!");

	m_levelManager.LoadLevel("Scripts/Levels/Tutorial.lua");
	m_columns = m_levelManager.GetColumns();
	m_rows = m_levelManager.GetRows();

	int cellWidth = m_windowWidth / m_columns;
	int cellHeight = m_windowHeight / m_rows;
	std::pair<int, int> heroPos = m_levelManager.GetHeroPos();

	m_beatPause = m_levelManager.GetBeatPause();
	m_beatBuffer = m_levelManager.GetBeatBuffer();

	m_gridItems.resize(m_rows);
	GridItem* temp;

	double x, y;
	for (int i = 0; i < m_rows; ++i)
	{
		m_gridItems.push_back(std::vector<GridItem*>());
		m_gridItems[i].resize(m_columns);
		y = i*cellHeight;
		for (int j = 0; j < m_columns; ++j)
		{
			x = j*cellWidth;
			temp = new GridItem(x, y, cellWidth, cellHeight);
			if (i == heroPos.second && j == heroPos.first)
			{
				temp->AddHero(m_levelManager.GetHero());
				temp->SetColor(sf::Color::Green);
			}

			m_gridItems[i][j] = temp;
		}
	}
	sf::SoundBuffer buffer;
	buffer.loadFromFile("test.wav");
	m_beatSound.setBuffer(buffer);

	m_gameClock.restart();
	m_on = true;

	m_gameState = GameState::Playing;
	while (m_gameState != GameState::Exiting)
	{
		Update();
		Draw();
	}
}

int Game::GetElapsedTime()
{
	return m_gameClock.restart().asMilliseconds();
}

void Game::AbilityUsed(int row, int column, bool tap, float angle)
{
	//if (m_timeSinceBeat > m_beatBuffer && m_timeSinceBeat < m_beatLength - m_beatBuffer)
	//{
	//	printf("OFF BEAT: %i \n", m_timeSinceBeat);
	//	return;
	//}
	//
	m_levelManager.UseAbility(tap, angle);
}

void Game::RotateHero(float direction)
{
	//if (m_timeSinceBeat > m_beatBuffer && m_timeSinceBeat < m_beatLength - m_beatBuffer)
	//{
	//	printf("OFF BEAT: %i \n", m_timeSinceBeat);
	//	return;
	//}

	m_levelManager.RotateHero(direction);
}

void Game::MoveHero(int row, int column, float direction)
{
	//if (m_timeSinceBeat > m_beatBuffer && m_timeSinceBeat < m_beatLength - m_beatBuffer)
	//{
	//	printf("OFF BEAT: %i \n", m_timeSinceBeat);
	//	return;
	//}

	m_gridItems[row][column]->RemoveHero();
	m_gridItems[row][column]->SetColor(sf::Color::White);
	auto newPos = m_levelManager.MoveHero(direction);

	m_gridItems[newPos.second][newPos.first]->AddHero(m_levelManager.GetHero());
	m_gridItems[newPos.second][newPos.first]->SetColor(sf::Color::Green);
}

void Game::Update()
{
	printf("Starting Update \n");

	int frames = 0;
	int time = GetElapsedTime();

	m_remainingTime += time;
	m_timeSinceBeat += time;

	while ((m_remainingTime > m_minTimestep) && (frames < m_maxFrames))
	{

		ProcessEvents();

		m_remainingTime -= m_minTimestep;

		frames++;
	}
	printf("Finished Events \n");
	if (m_timeSinceBeat >= m_beatPause)
	{
		printf("*********Beat, updating grid \n");
		for (int i = 0; i < m_rows; ++i)
			for (int j = 0; j < m_columns; ++j)
				m_gridItems[i][j]->Update();
		printf("*********Beat Time: %i \n", m_timeSinceBeat);
		m_beatSound.play();
		m_timeSinceBeat = 0;
		m_levelManager.Update();
	}

	printf("End Update \n");
}

void Game::ProcessEvents()
{
	sf::Event event;

	printf("Processing events \n");
	while (m_renderWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_gameState = Exiting;
		else if (event.type == sf::Event::MouseButtonPressed)
			MousePressEvent(event);
		else if (event.type == sf::Event::MouseMoved)
			MouseMoveEvent(event);
		else if (event.type == sf::Event::MouseButtonReleased)
			MouseReleaseEvent(event);
		else if (event.type == sf::Event::KeyPressed)
			KeyPressEvent(event);
		printf("Processing events in loop \n");
	}
}

void Game::Draw()
{
	m_renderWindow.clear();

	for (int i = 0; i < m_rows; ++i)
		for (int j = 0; j < m_columns; ++j)
			m_gridItems[i][j]->Draw(m_renderWindow);

	auto drawData = m_levelManager.GetDrawingData();
	for (int i = 0; i < drawData.size(); ++i)
	{
		if (drawData[i].row < 0 || drawData[i].row >= m_rows || drawData[i].column < 0 || drawData[i].column >= m_columns)
			continue;

		m_gridItems[drawData[i].row][drawData[i].column]->SetColor(sf::Color(drawData[i].R, drawData[i].G, drawData[i].B));
	}

	m_renderWindow.display();
}

void Game::MousePressEvent(const sf::Event& event)
{
	glm::vec2 clickPosOnGrid = MapCoordToGrid(event.mouseButton.x, event.mouseButton.y);

	if (m_gridItems[clickPosOnGrid.y][clickPosOnGrid.x]->HasHero() && event.mouseButton.button == sf::Mouse::Left)
	{
		m_trackMouse = true;
		m_firstClick = glm::vec2(event.mouseButton.x, event.mouseButton.y);
	}
}
void Game::MouseMoveEvent(const sf::Event& event)
{
	if (!m_trackMouse)
		return;

	m_mousePositions.push_back(glm::vec2(event.mouseMove.x, event.mouseMove.y) - m_firstClick);

}
void Game::MouseReleaseEvent(const sf::Event& event)
{
	glm::vec2 clickPos = MapCoordToGrid(m_firstClick.x, m_firstClick.y);

	if (event.mouseButton.button == sf::Mouse::Left)
	{
		if (m_mousePositions.size() <= 3)
			AbilityUsed(clickPos.y, clickPos.x, true, 0);
		else
		{
			glm::vec2 finalVec(glm::vec2(event.mouseButton.x, event.mouseButton.y) - m_firstClick);
			glm::vec2 finalVecNorm = glm::normalize(finalVec);
			float averageDistance = 0;

			for (int i = 0; i < m_mousePositions.size(); ++i)
				averageDistance += PointDistanceToVec(m_mousePositions[i], finalVec);
			averageDistance /= m_mousePositions.size();

			if (averageDistance <= 5)
			{
				float angle = std::atan2(finalVec.y, finalVec.x);

				if (angle < 0)
					angle += 2 * M_PI;

				MoveHero(clickPos.y, clickPos.x, angle);
			}
			else
			{
				double direction = -M_PI_2;
				glm::vec2 first(m_mousePositions[1] - m_firstClick);
				glm::vec2 last(m_mousePositions[m_mousePositions.size() - 2] - m_firstClick);
				first = glm::normalize(first);
				last = glm::normalize(last);

				double cross = first.x * last.y - first.y * last.x;
				if (cross < 0)
					direction = M_PI_2;

				RotateHero(direction);
			}
		}
	}
	else if (event.mouseButton.button == sf::Mouse::Right)
		if (m_mousePositions.size() <= 3)
			AbilityUsed(clickPos.y, clickPos.x, false, 0);

	m_mousePositions.clear();
	m_trackMouse = false;
}

void Game::KeyPressEvent(const sf::Event& event)
{
	if (event.key.code == sf::Keyboard::M)
		m_mute = !m_mute;

	if (event.key.code == sf::Keyboard::R)
		Restart();
}

glm::vec2 Game::MapCoordToGrid(double x, double y)
{
	glm::vec2 result;

	result.x = std::floor(x / (m_windowWidth / m_columns));
	result.y = std::floor(y / (m_windowHeight / m_rows));

	return result;
}

double Game::PointDistanceToVec(const glm::vec2& point, const glm::vec2& line)
{
	double result;

	result = std::abs((point.x * line.y - point.y * line.x) / glm::length(line));

	return result;
}

void Game::Restart()
{
	m_levelManager.Clear();

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
	
	Start();
}