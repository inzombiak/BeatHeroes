ILevel =
{
	_name = "NULL",
	_tmxPath = "NULL",
	_rows = -1,
	_columns = -1,
	_beatPause = -1,
	_beatBuffer = -1,
	_heroX = -1,
	_heroY = -1,
	_enemies = {}
}

function ILevel:Create()
	local b = {}
    setmetatable(b, self)
	b.__index = self
    return b
end

function ILevel:GetName()
	return self._name
end

function ILevel:GetTmxPath()
	return self._tmxPath
end

function ILevel:GetRows()
	return self._rows
end

function ILevel:GetColumns()
	return self._columns
end

function ILevel:GetBeatPause()
	return self._beatPause
end

function ILevel:GetBeatBuffer()
	return self._beatBuffer
end

function ILevel:GetHeroX()
	return self._heroX
end

function ILevel:GetHeroY()
	return self._heroY
end

function ILevel:GetEnemies()
	return self._enemies
end

function ILevel:GetEnemy(i)
	print(self._enemies[i]:GetName())
	return self._enemies[i]
end

function ILevel:KillEnemy(i)
	if(i > #self._enemies) then
		print("INVALID ENEMY TO REMOVE, INDEX: ", i)
	end
	
	self._enemies[i]:Kill()
	table.remove(self._enemies, i);
end