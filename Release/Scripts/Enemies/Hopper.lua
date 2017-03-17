require "Scripts/Enemies/IEnemy"

Hopper =
{
	_name = "Hopper",
	_id = -1,
	_health = 3,
	_damage = 1,
	_attackPattern = {},
	_direction = 0
}
Hopper.__index = Hopper

function Hopper:Create(newID)
	local b = {}
    setmetatable(b, Hopper)
	setmetatable(Hopper,  { __index = IEnemy})
	self._id = newID
	self:Init()
	math.randomseed(os.time())
    return b
end

function Hopper:Init()
	self._attackPattern[1] = {0, 0}
	self._animPath = "Images/Enemies/Hopper/Hopper_Anim.anim"
	self._currAnimName = "idle"
end

function Hopper:Update()
	--[[local dice = math.random(10)
	if(dice >= 9) then
		self._direction = self._direction + math.pi/ 2
	end]]--
	self._posX = self._posX + math.cos(self._direction)
	self._posY = self._posY + math.sin(self._direction)
end

function Hopper:ContactWithStatic()
	self._direction = self._direction + math.pi / 2
	if(self._direction >= math.pi * 2) then
		self._direction = self._direction - math.pi * 2;
	end
end