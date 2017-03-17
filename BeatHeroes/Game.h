#ifndef GAME_H
#define GAME_H

#include <vector>
#include <glm/glm.hpp>
#include "LevelManager.h"

enum GameState
{
	Initializing = 0,
	Playing = 1,
	Paused = 2,
	Exiting = 3,
};

enum ControlScheme
{
    SplitScreen = 0,
    Regular = 1,
};

class Game
{

public:
	~Game();
	void Start();
    void Update();

	void MousePressEvent(const sf::Event& event);
	void MouseMoveEvent(const sf::Event& event);
	void MouseReleaseEvent(const sf::Event& event);
	void KeyPressEvent(const sf::Event& event);
    void SetScreenSize(int width, int height);
    void SetControlScheme(ControlScheme scheme);
    
    void ToggleMute();
    void ToggleControls();
    void ToggleBeat();
//    void Swipe(double direction);
//    void SwipeCurve(bool isClockwise);
//    void Tap();
//    void DoubleTap();

private:

	void Destroy();
	void Restart();
	void ProcessEvents(); 
	void Draw();
    void HandleRegularSchemeInput(const glm::vec2& pos, bool doubleTap);
    void HandleSplitScreenInput(const glm::vec2& pos, bool doubleTap);
    void HandleLeftScreenInput(const glm::vec2& pos, bool doubleTap);
    void HandleRightScreenInput(const glm::vec2& pos, bool doubleTap);
	double GetElapsedTime();
//	void KeyPressEvent(const sf::Event& event);
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
	sf::Clock m_gameClock;
	int m_windowWidth = 32 * 35;
	int m_windowHeight = 32*15;

	int m_rows, m_columns;

    const double m_minTimestep = (1.f / 60.f);
	const int m_maxFrames = 5;
	double  m_remainingTime = 0;
	double m_timeSinceBeat = 0;
    double m_lastTime = 0;
	double m_beatPause= -1;
	double m_beatBuffer = -1;
	bool m_on = true;
	bool m_mute = false;
    bool m_enableBeat = false;
    
    ControlScheme m_controlScheme = SplitScreen;
	LevelManager m_levelManager;
};

#endif // MAINWINDOW_H
