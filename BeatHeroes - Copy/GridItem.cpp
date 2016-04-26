#include "GridItem.h"
#include <QPainter>

GridItem::GridItem(int row, int column, QWidget *parent)
	: QWidget(parent)
{
	m_row = row;
	m_column = column;
	m_hero = 0;
	ui.setupUi(this);


}

GridItem::~GridItem()
{

}

void GridItem::resizeEvent(QResizeEvent* event)
{

}

void GridItem::paintEvent(QPaintEvent *) 
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

	if (m_hero)
	{
		int dirRectWidth = this->size().width() / 5;
		int dirRectHeight = this->size().height() / 5;
		int xPos = (this->size().width() - dirRectWidth) * (1 + std::cos(m_hero->GetDirection())) / 2;
		int yPos = (this->size().height() - dirRectHeight) * (1 + std::sin(m_hero->GetDirection())) / 2;
		m_directionRect = QRect(xPos, yPos, dirRectWidth, dirRectHeight);


		p.fillRect(m_directionRect, QBrush(Qt::red));
	}
}

void GridItem::mousePressEvent(QMouseEvent* event)
{
	if (m_hero)
		if (event->button() == Qt::LeftButton | Qt::RightButton)
		{
			m_click = event->pos();
			m_trackMouse = true;
		}
			
}

void GridItem::mouseMoveEvent(QMouseEvent* event)
{
	if (m_trackMouse && m_hero)
		if (event->buttons() == Qt::LeftButton)
		{
			m_mousePositions.push_back(QVector2D(event->pos() - m_click));
		}
}

void GridItem::mouseReleaseEvent(QMouseEvent* event)
{
	if (m_trackMouse && m_hero)
	{
		/*if (event->button() == Qt::LeftButton)
		{
			if (m_mousePositions.size() <= 3)
				AbilityUsedSignal(m_row, m_column, true);
			else
			{
				QVector2D finalVec(event->pos() - m_click);
				QVector2D finalVecNorm = finalVec.normalized();
				float averageDistance;

				for (int i = 0; i < m_mousePositions.size(); ++i)
					averageDistance += m_mousePositions[i].distanceToLine(finalVec, finalVecNorm);
				averageDistance /= m_mousePositions.size();


				if (averageDistance <= 5)
				{
					QVector2D vec(event->pos() - m_click);
					auto distance = vec.length();

					vec.normalize();
					float angle = std::atan2(vec.y(), vec.x());

					if (angle < 0)
						angle += 2 * M_PI;

					AbilityUsedSignal(m_row, m_column, false, angle);
				}
				else
				{
					double direction = -M_PI_2;
					QVector2D first(m_mousePositions[1] - QVector2D(m_click));
					QVector2D last(m_mousePositions[m_mousePositions.size() - 2] - QVector2D(m_click));
					first.normalize();
					last.normalize();
					double cross = first.x() * last.y() - first.y() * last.x();
					if (cross < 0)
						direction = M_PI_2;

					RotateHeroSignal(direction);
				}
			}
		}
		else if (event->button() == Qt::RightButton)
			if (m_mousePositions.size() <= 3)
				AbilityUsedSignal(m_row, m_column, false);
*/

		if (event->button() == Qt::LeftButton)
		{
			if (m_mousePositions.size() <= 3)
				AbilityUsedSignal(m_row, m_column, true);
			else
			{
				QVector2D finalVec(event->pos() - m_click);
				QVector2D finalVecNorm = finalVec.normalized();
				float averageDistance = 0;

				for (int i = 0; i < m_mousePositions.size(); ++i)
					averageDistance += m_mousePositions[i].distanceToLine(finalVec, finalVecNorm);
				averageDistance /= m_mousePositions.size();


				if (averageDistance <= 5)
				{
					QVector2D vec(event->pos() - m_click);
					vec.normalize();
					float angle = std::atan2(vec.y(), vec.x());

					if (angle < 0)
						angle += 2 * M_PI;

					MoveHeroSignal(m_row, m_column, angle);
				}
				else
				{
					double direction = -M_PI_2;
					QVector2D first(m_mousePositions[1] - QVector2D(m_click));
					QVector2D last(m_mousePositions[m_mousePositions.size() - 2] - QVector2D(m_click));
					first.normalize();
					last.normalize();
					double cross = first.x() * last.y() - first.y() * last.x();
					if (cross < 0)
						direction = M_PI_2;

					RotateHeroSignal(direction);
				}
			}
		}
		else if (event->button() == Qt::RightButton)
			if (m_mousePositions.size() <= 3)
				AbilityUsedSignal(m_row, m_column, false);

		m_mousePositions.clear();
		m_trackMouse = false;
	}
}