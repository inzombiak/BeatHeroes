#ifndef GRIDITEM_H
#define GRIDITEM_H

#define _USE_MATH_DEFINES
#include <cmath>

#include <QWidget>
#include <QMouseEvent>

#include "IHero.h"
#include "ui_GridItem.h"

class GridItem : public QWidget
{
	Q_OBJECT

public:
	GridItem(int row, int column, QWidget *parent = 0);
	~GridItem();
	void Update()
	{
		setStyleSheet("background-color:white;");
		if (m_hero)
			setStyleSheet("background-color:green;");
	}

	void AddHero(IHero* hero)
	{
		m_hero = hero;
	}

	void RemoveHero()
	{
		m_hero = 0;
	}

signals:
	void AbilityUsedSignal(int row, int column, bool singleTap, float angle = 0);
	void RotateHeroSignal(float angle);
	void MoveHeroSignal(int row, int column, float direction);

protected:
	void resizeEvent(QResizeEvent* event);
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	
private:
	Ui::GridItem ui;

	QPoint m_click;

	IHero* m_hero = 0;
	int m_row;
	int m_column;
	bool m_trackMouse;

	std::vector<QVector2D> m_mousePositions;

	QRect m_directionRect;
};

#endif
