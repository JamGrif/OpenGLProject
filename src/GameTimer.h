#pragma once


class GameTimer
{
public:

	GameTimer();
	~GameTimer();

	void startGameTimer();
	void stopGameTimer();

	void updateGameTimer();

	double getDeltaTime();

private:

	// Delta time
	double		m_deltaTime;
	double		m_lastFrame;

	double		m_previousTime;
	int			m_frameCount;
	double		m_currentFrame;

};
