#include "mainwindow.h"
#include "GridItem.h"
#include "LuaWrapper\LuaWrapper.h"

#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	GenerateGrid();
}

MainWindow::~MainWindow()
{

}

void MainWindow::GenerateGrid()
{
	m_gameState = GameState::Initializing;
	m_levelManager.LoadLevel("Scripts/Levels/Tutorial.lua");
	m_columns = m_levelManager.GetColumns();
	m_rows = m_levelManager.GetRows();

	int cellWidth = ui.centralWidget->size().width() / m_columns;
	int cellHeight = ui.centralWidget->size().height() / m_rows;
	std::pair<int, int> heroPos = m_levelManager.GetHeroPos();

	m_beatLength = m_levelManager.GetBeatLength();
	m_beatBuffer = m_levelManager.GetBeatBuffer();

	m_gridItems.resize(m_rows);
	GridItem* temp;

	for (int i = 0; i < m_rows; ++i)
	{
		m_gridItems.push_back(std::vector<GridItem*>());
		m_gridItems[i].resize(m_columns);
		for (int j = 0; j < m_columns; ++j)
		{
			temp = new GridItem(i, j, this);
			temp->resize(cellWidth, cellHeight);
			
			temp->setAutoFillBackground(true);
			temp->setStyleSheet("background-color:white;");

			if (i == heroPos.second && j == heroPos.first)
			{
				temp->AddHero(m_levelManager.GetHero());
				temp->setStyleSheet("background-color:red;");
				QObject::connect(temp, SIGNAL(AbilityUsedSignal(int, int, bool, float)), this, SLOT(AbilityUsed(int, int, bool, float)));
				QObject::connect(temp, SIGNAL(RotateHeroSignal(float)), this, SLOT(RotateHero(float)));
				QObject::connect(temp, SIGNAL(MoveHeroSignal(int, int, float)), this, SLOT(MoveHero(int, int, float)));
			}

			ui.m_grid->addWidget(temp, i, j);
			
			m_gridItems[i][j] = temp;
		}
	}

	m_beatSound = new QSound("test.wav", this);

	m_on = true;
	m_timer.start();
	m_lastTime = 0;
	m_gameState = GameState::Playing;
}

void MainWindow::AbilityUsed(int row, int column, bool tap, float angle)
{
	m_levelManager.UseAbility(tap, angle);
}

void MainWindow::RotateHero(float direction)
{
	m_levelManager.RotateHero(direction);
}

void MainWindow::MoveHero(int row, int column, float direction)
{
	m_gridItems[row][column]->RemoveHero();
	QObject::disconnect(m_gridItems[row][column], SIGNAL(AbilityUsedSignal(int, int, bool, float)), this, SLOT(AbilityUsed(int, int, bool, float)));
	QObject::disconnect(m_gridItems[row][column], SIGNAL(RotateHeroSignal(float)), this, SLOT(RotateHero(float)));
	QObject::disconnect(m_gridItems[row][column], SIGNAL(MoveHeroSignal(int, int, float)), this, SLOT(MoveHero(int, int, float)));

	auto newPos = m_levelManager.MoveHero(direction);

	m_gridItems[newPos.second][newPos.first]->AddHero(m_levelManager.GetHero());
	QObject::connect(m_gridItems[newPos.second][newPos.first], SIGNAL(AbilityUsedSignal(int, int, bool, float)), this, SLOT(AbilityUsed(int, int, bool, float)));
	QObject::connect(m_gridItems[newPos.second][newPos.first], SIGNAL(RotateHeroSignal(float)), this, SLOT(RotateHero(float)));
	QObject::connect(m_gridItems[newPos.second][newPos.first], SIGNAL(MoveHeroSignal(int, int, float)), this, SLOT(MoveHero(int, int, float)));
}

void MainWindow::UpdateGame()
{
	double newTime =  m_timer.restart();
	int frames = 0;
	
	m_accumulator += newTime;
	printf("Accumulator: %lf, New time: %lf \n", m_accumulator, newTime);
	while (m_accumulator >= m_timeStep && frames < m_maxFrames)
	{
		for (int i = 0; i < m_rows; ++i)
			for (int j = 0; j < m_columns; ++j)
				m_gridItems[i][j]->Update();
		//printf("Update gridItems: %i \n", m_timer.elapsed());

		m_gridItems[0][0]->setStyleSheet("background-color:yellow;");

		auto drawData = m_levelManager.GetDrawingData();
		QString s;
		for (int i = 0; i < drawData.size(); ++i)
		{
			if (drawData[i].row < 0 || drawData[i].row >= m_rows || drawData[i].column < 0 || drawData[i].column >= m_columns)
				continue;

			s = QString("background-color:rgb(%1,%2,%3);").arg(drawData[i].R).arg(drawData[i].G).arg(drawData[i].B);

			m_gridItems[drawData[i].row][drawData[i].column]->setStyleSheet(s);
		}
		
		printf("Time Since Beat: %lf, Beat Length: %i \n", m_timeSinceLastBeat, m_beatLength);

		if (m_timeSinceLastBeat >= m_beatLength)
		{
			m_levelManager.Update();
			if (!m_mute)
				m_beatSound->play();
			
			m_timeSinceLastBeat = 0;
			printf("BEAT \n");
		}
			
		m_accumulator -= m_timeStep;
		m_timeSinceLastBeat += m_timeStep;
		frames++;
	}

	m_lastTime = newTime;
	//m_accumulator = 0;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_M)
		m_mute = !m_mute;

	if (event->key() == Qt::Key_R)
		Restart();
}

void MainWindow::Restart()
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

	delete m_beatSound;

	GenerateGrid();
}