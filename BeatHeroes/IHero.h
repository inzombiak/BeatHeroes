#pragma once

#include "AnimationData.h"

#include <SFML\Graphics.hpp>
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
    int m_tempOffsetX = 0, m_tempOffsetY = 0;
    IHero(unsigned int id, std::string filePath);
	~IHero();

	AbilityInfo Tap(double angle, bool useOwnDirection);
	AbilityInfo DoubleTap(double angle, bool useOwnDirection);
	std::pair<int, int> GetCooldowns() const;
	std::pair<int, int> GetBeatsPassed() const;
	void Update();
	void UpdateRender();
	void Draw(sf::RenderWindow& rw);
	void Rotate(double angle);
	double GetDirection()
	{
		return m_direction;
	}
	void SetPos(const std::pair<int, int>& newPos)
	{
		m_pos = newPos;
       // UpdateSKSpritePos();
		m_sprite.setPosition((float)m_pos.first * 32, (float)m_pos.second * 32);
	}

	std::pair<int, int> Move(double direction);
	const std::pair<int, int>& GetPos() const
	{
		return m_pos;

	}

protected:
	IAbility* m_sglClickAb;
	IAbility* m_dblClickAb;

	void LoadSpriteSheet();
	void LoadAnimationFile();
	void UpdateAnimation();
    //void UpdateSKSprite(Animation::IntRect rect);
    //void UpdateSKSpritePos()
    //{
    //     [m_sprite setPosition:CGPointMake(m_tempOffsetX+(float)m_pos.first * 32, m_tempOffsetY - (float)m_pos.second * 32)];
    //}
	double m_direction = 0;
	unsigned int m_id;
	std::pair<int, int> m_pos;
	std::string m_filePath;
    
    sf::Sprite m_sprite;
	sf::Texture m_spriteSheet;
	sf::IntRect m_textureRect;
    int m_tileWidth = 32;
    int m_tileHeight = 32;
	int m_counter = 0; //Number of times Update has bee called
	const int m_maxCounter = 4; //Animations moves forward by a a frame when counters reaches this number

	Animation::Animation m_currentAnimation; //Current naimation being played
	std::map<std::string, std::string> m_messageToAnimation; //Mapping for message to animation name
	std::map<std::string, Animation::SpriteDefinition> m_spriteDefinitions; //Contains texture bounds for each sprite
	std::map<std::string, Animation::Animation> m_animations; //Contains animaitons
};

