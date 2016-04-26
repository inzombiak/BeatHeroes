#pragma once

#include <string>
#include <vector>
#include <memory>

class LuaObject;
class Enemy
{
public:
	Enemy(unsigned int id, std::shared_ptr<LuaObject> obj);
	~Enemy();

	void Init(unsigned int id, std::shared_ptr<LuaObject> obj);
	void Kill();

	void Update();
	std::pair<int, int> GetPos() const;
	void SetPos(int newPosX, int newPosY);

	int GetHealth() const;
	void SetHealth(int amount);
	int GetDamage() const;
	const std::string& GetName() const;
	std::vector<std::pair<int, int>> GetPattern() const;

	void ContactWithStatic() const;

	bool IsAlive() const
	{
		return m_isAlive;
	}

private:
	unsigned int m_id;
	int m_health;
	bool m_isAlive;
	std::shared_ptr<LuaObject> m_luaObj;
};

