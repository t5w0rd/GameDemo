if __OBJECT_LIBRARY__ then return end
__OBJECT_LIBRARY__ = true


ObjectLibrary = class()
function ObjectLibrary:ctor(step, addFunc)
	self.step = step
	self.id = step
	self.id2 = 1
	self.addFunc = addFunc
end

function ObjectLibrary:setId(id)
    self.id = id
end

function ObjectLibrary:add(obj)
	-- TODO: add
	local id = self.id
	self:addFunc(id, obj)
	self.id = self.id + self.step
	self.id2 = 1
	return id
end

function ObjectLibrary:add2(obj)
	local id = self.id + self.id2
	self:addFunc(id, obj)
	self.id2 = self.id2 + 1
	return id
end
