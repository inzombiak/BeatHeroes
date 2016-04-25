#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QElapsedTimer>
#include <QSound>
#include "ui_mainwindow.h"
#include "LevelManager.h"

enum GameState
{
	Initializing = 0,
	Playing = 1,
	Closing = 2,
};

class GridItem;
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void AbilityUsed(int row, int column, bool tap, float angle);
	void RotateHero(float direction);
	void MoveHero(int row, int column, float direction);
	void UpdateGame();
	GameState GetGameState() const
	{
		return m_gameState;
	}
protected:
	void keyPressEvent(QKeyEvent* event);

private:
	void GenerateGrid();
	void Restart();

	int m_rows, m_columns;
	int m_beatLength = -1;
	int m_beatBuffer = -1;
	bool m_on;
	bool m_mute = true;

	QElapsedTimer m_timer;
	double m_timeStep =	60;
	double m_accumulator = 0;
	double m_lastTime = 0;
	double m_timeSinceLastBeat = 0;
	int m_maxFrames = 10;
	
	GameState m_gameState;

	std::vector<std::vector<GridItem*>> m_gridItems;

	Ui::MainWindowClass ui;
	QSound* m_beatSound;

	LevelManager m_levelManager;

};

#endif // MAINWINDOW_H
