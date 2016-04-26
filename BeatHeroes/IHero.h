#pragma once
#include <vector>

struct AbilityInfo
{
	int damage;
	std::vector<std::pair<int, int>> pattern;
};

class IAbility;
class IHero
{
public:
	IHero(unsigned int id, std::string filePath);
	~IHero();

	AbilityInfo Tap();
	AbilityInfo DoubleTap();
	std::pair<int, int> GetCooldowns() const;
	std::pair<int, int> GetBeatsPassed() const;
	void Update();
	void Rotate(double angle)
	{
		m_direction += angle;
	}
	double GetDirection()
	{
		return m_direction;
	}
	void SetPos(const std::pair<int, int>& newPos)
	{
		m_pos = newPos;
	}

	std::pair<int, int> Move(double direction);
	const std::pair<int, int>& GetPos() const
	{
		return m_pos;
	}

protected:
	IAbility* m_sglClickAb;
	IAbility* m_dblClickAb;
	double m_direction = 0;
	unsigned int m_id;
	std::pair<int, int> m_pos;
	std::string m_filePath;
};

