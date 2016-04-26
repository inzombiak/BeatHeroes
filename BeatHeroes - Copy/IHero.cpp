#define _USE_MATH_DEFINES
#include <cmath> 
#include "IHero.h"
#include "IAbility.h"

#include "LuaWrapper\LuaWrapper.h"

IHero::IHero(unsigned int id, std::string filepath)
{
	m_id = id;
	m_filePath = filepath;
	
	LuaWrapper::GetInstance().LoadScript(filepath + std::to_string(id), m_filePath);
	m_sglClickAb = new IAbility(1, LuaWrapper::GetInstance().GetGlobalValue<std::string>(filepath + std::to_string(id), "singleClick"));
	m_dblClickAb = new IAbility(2, LuaWrapper::GetInstance().GetGlobalValue<std::string>(filepath + std::to_string(id), "doubleClick"));
}


IHero::~IHero()
{
	if (m_sglClickAb)
	{
		delete m_sglClickAb;
		m_sglClickAb = 0;
	}

	if (m_dblClickAb)
	{
		delete m_dblClickAb;
		m_dblClickAb = 0;
	}
	
}

std::pair<int, int> IHero::GetCooldowns() const
{
	return std::pair<int, int>(m_sglClickAb->GetCooldown(), m_dblClickAb->GetCooldown());
}

std::pair<int, int> IHero::GetBeatsPassed() const
{
	return std::pair<int, int>(m_sglClickAb->GetBeats(), m_dblClickAb->GetBeats());
}

void IHero::Update()
{
	m_sglClickAb->Update();
	m_dblClickAb->Update();
}

AbilityInfo IHero::Tap()
{
	AbilityInfo result;

	m_sglClickAb->Use(result.pattern);

	float angleSin = std::sin(m_direction);
	float angleCos = std::cos(m_direction);

	if (std::abs(angleCos) < 0.001)
		angleCos = 0;
	if (std::abs(angleSin) < 0.001)
		angleSin = 0;

	for (int i = 0; i < result.pattern.size(); ++i)
	{
		int posX = (result.pattern[i].first * angleCos - result.pattern[i].second * angleSin);
		int posY = (result.pattern[i].first * angleSin + result.pattern[i].second * angleCos);
		result.pattern[i] = std::pair<int, int>(posX, posY);
	}

	result.damage = m_sglClickAb->GetDamage();

	return result;
}

AbilityInfo IHero::DoubleTap()
{
	AbilityInfo result;

	m_dblClickAb->Use(result.pattern);

	float angleSin = std::sin(m_direction);
	float angleCos = std::cos(m_direction);

	if (std::abs(angleCos) < 0.001)
		angleCos = 0;
	if (std::abs(angleSin) < 0.001)
		angleSin = 0;

	for (int i = 0; i < result.pattern.size(); ++i)
	{
		int posX = (result.pattern[i].first * angleCos - result.pattern[i].second * angleSin);
		int posY = (result.pattern[i].first * angleSin + result.pattern[i].second * angleCos);
		result.pattern[i] = std::pair<int, int>(posX, posY);
	}

	result.damage = m_dblClickAb->GetDamage();

	return result;
}

std::pair<int, int> IHero::Move(float direction)
{
	float clampedAngle = std::round(direction / M_PI_2) * M_PI_2;
	if (clampedAngle >= 2*M_PI);
		clampedAngle -= 2*M_PI;

	int cos = std::cos(clampedAngle);
	int sin = std::sin(clampedAngle);

	m_pos.first += cos;
	m_pos.second += sin;

	m_direction = clampedAngle;

	return m_pos;
}
