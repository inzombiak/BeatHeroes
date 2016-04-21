require "Scripts/Enemies/IEnemy"

FireSpinner =
{
	_name = "FireSpinner",
	_id = -1,
	_health = 3,
	_damage = 1,
	_attackPattern = {},
	
	_direction = 0
}

FireSpinner.__index = FireSpinner

function FireSpinner:Create(newID)
	local b = {}
    setmetatable(b, FireSpinner)
	setmetatable(FireSpinner,  { __index = IEnemy})
	self._id = newID
	self:Init()
	print("Firespinner ready")
    return b
end

function FireSpinner:Init()
	self._attackPattern[1] = {1, 0}
	self._attackPattern[2] = {2, 0}
	self._attackPattern[3] = {3, 0}
	self._attackPattern[4] = {4, 0}
end


function FireSpinner:Update()
	local newDir = self._direction + math.pi/2
	self._direction = newDir

	if(self._direction >= 2*math.pi) then
		self._direction = self._direction - 2*math.pi
	end
	
	local dirSin = math.sin(self._direction)
	local dirCos = math.cos(self._direction)
	
	if(math.abs(dirSin) < 0.00001) then
		dirSin = 0
	end
	if(math.abs(dirCos) < 0.00001) then
		dirCos = 0
	end

	for i = 1, #self._attackPattern do
		local apY = self._attackPattern[i][1] * dirCos - self._attackPattern[i][2] * dirSin
		local apX = self._attackPattern[i][1] * dirSin + self._attackPattern[i][2] * dirCos
		self._attackPattern[i][1] = apX 
		self._attackPattern[i][2] = apY
	end
end
