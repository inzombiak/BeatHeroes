#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <glm.hpp>
#include "LevelManager.h"

enum GameState
{
	Initializing = 0,
	Playing = 1,
	Exiting = 2,
};

class Game
{

public:
	void Start();


//public slots:
//	void AbilityUsed(int row, int column, bool tap, float angle);
//	void RotateHero(float direction);
//	void MoveHero(int row, int column, float direction);
//	void Update();
//	void Beat();
//

private:

	void Destroy();
	void Restart();
	void Update();
	void ProcessEvents(); 
	void Draw();
	int GetElapsedTime();

	void MousePressEvent(const sf::Event& event);
	void MouseMoveEvent(const sf::Event&event);
	void MouseReleaseEvent(const sf::Event& event);
	void KeyPressEvent(const sf::Event& event);
	glm::vec2 MapCoordToGrid(double x, double y);
	double PointDistanceToVec(const glm::vec2& point, const glm::vec2& line);

	void AbilityUsed(int row, int column, bool tap, float angle);
	void RotateHero(float direction);
	void MoveHero(int row, int column, float direction);

	bool m_trackMouse;
	glm::vec2 m_firstClick;
	std::vector<glm::vec2> m_mousePositions;
	GameState m_gameState;

	sf::RenderWindow m_renderWindow;
	int m_windowWidth = 320;
	int m_windowHeight = 320;

	int m_rows, m_columns;

	sf::Clock m_gameClock;
	const int m_minTimestep = sf::seconds(1.f / 60.f).asMilliseconds();
	const int m_maxFrames = 5;
	int m_remainingTime = 0;
	int m_timeSinceBeat = 0;

	sf::Sound m_beatSound;
	int m_beatPause= -1;
	int m_beatBuffer = -1;
	bool m_on;
	bool m_mute = true;

	LevelManager m_levelManager;
};

#endif // MAINWINDOW_H
