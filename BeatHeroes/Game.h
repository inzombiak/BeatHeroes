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
	Paused = 2,
	Exiting = 3,
};

class Game
{

public:
	~Game();
	void Start();


private:

	void Destroy();
	void Restart();
	void Update();
	void ProcessEvents(); 
	void Draw();
	double GetElapsedTime();

	void MousePressEvent(const sf::Event& event);
	void MouseMoveEvent(const sf::Event&event);
	void MouseReleaseEvent(const sf::Event& event);
	void KeyPressEvent(const sf::Event& event);
	glm::vec2 MapCoordToGrid(double x, double y);
	double PointDistanceToVec(const glm::vec2& point, const glm::vec2& line);

	void AbilityUsed(int row, int column, bool tap, double angle);
	void RotateHero(double direction);
	void MoveHero(int row, int column, double direction);

	bool m_trackMouse;
	glm::vec2 m_firstClick;
	std::vector<glm::vec2> m_mousePositions;
	GameState m_gameState;

	sf::RenderWindow m_renderWindow;
	int m_windowWidth = 320;
	int m_windowHeight = 320;

	int m_rows, m_columns;

	sf::Clock m_gameClock;
	sf::Clock m_testClock;
	const double m_minTimestep = sf::seconds(1.f / 60.f).asSeconds();
	const int m_maxFrames = 5;
	double  m_remainingTime = 0;
	double m_timeSinceBeat = 0;

	sf::Sound m_beatSound;
	double m_beatPause= -1;
	double m_beatBuffer = -1;
	bool m_on;
	bool m_mute = true;

	LevelManager m_levelManager;
};

#endif // MAINWINDOW_H
