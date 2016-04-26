#include "Enemy.h"
#include "LuaWrapper\LuaWrapper.h"
#include "LuaWrapper\LUAObject.h"

Enemy::Enemy(unsigned int id, std::shared_ptr<LuaObject> obj)
{
	Init(id, obj);
}

Enemy::~Enemy()
{

}

void Enemy::Update()
{
	if (!m_isAlive)
		return;

	m_luaObj->CallFunction("Update");
}

std::pair<int, int> Enemy::GetPos() const
{
	int x = m_luaObj->CallFunction<int>("GetPosX");
	int y = m_luaObj->CallFunction<int>("GetPosY");

	return std::pair<int, int>(x, y);
}

void Enemy::SetPos(int newPosX, int newPosY)
{
	m_luaObj->CallFunction<void>("SetPosX", newPosX);
	m_luaObj->CallFunction<void>("SetPosY", newPosY);
}

int Enemy::GetHealth() const
{
	return m_luaObj->CallFunction<int>("GetHealth");
}

void Enemy::SetHealth(int amount)
{
	m_luaObj->CallFunction<void>("SetHealth", amount);
}

int Enemy::GetDamage() const
{
	return m_luaObj->CallFunction<int>("GetDamage");
}

const std::string& Enemy::GetName() const
{
	return m_luaObj->GetName();
}

std::vector<std::pair<int, int>> Enemy::GetPattern() const
{
	return m_luaObj->CallFunctionVec<int, int>("GetAttackPattern");
}

void Enemy::Init(unsigned int id, std::shared_ptr<LuaObject> obj)
{
	m_isAlive = true;
	m_luaObj = obj;
	m_health = GetHealth();
	m_id = id;
}

void Enemy::Kill()
{
	m_isAlive = false;
	m_luaObj = nullptr;
	m_health = 0;
}

void Enemy::ContactWithStatic() const
{
	m_luaObj->CallFunction<void>("ContactWithStatic");
}