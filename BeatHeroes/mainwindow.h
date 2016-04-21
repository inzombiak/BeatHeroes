#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include <QSound>
#include "ui_mainwindow.h"
#include "LevelManager.h"

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
	void Update();
	void Beat();

protected:
	void keyPressEvent(QKeyEvent* event);

private:

	void Restart();
	int m_rows, m_columns;
	int m_beatLength = -1;
	int m_beatBuffer = -1;
	bool m_on;
	bool m_mute = true;
	void GenerateGrid();
	std::vector<std::vector<GridItem*>> m_gridItems;
	Ui::MainWindowClass ui;
	QTimer* m_gameStepTimer;
	QSound* m_beatSound;

	LevelManager m_levelManager;

};

#endif // MAINWINDOW_H
