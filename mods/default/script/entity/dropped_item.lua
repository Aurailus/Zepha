local function collides(entity)
    return zepha.get_block({
        x = math.floor(entity.pos.x),
        y = math.floor(entity.pos.y - 0.5),
        z = math.floor(entity.pos.z)}) ~= "air"
end

zepha.register_entity("default:test", {
    display = "model",
    display_object = "zeus:default:player",
    display_texture = "zeus:default:player",
    on_create = function(self)
--        self.object:set_scale(1/4)
    end,
    on_update = function(self)
--        local pos = self.object.pos
--        pos.z = pos.z + 0.08
--        if pos.z > 13 then
--            pos.z = 0
--        end
--        self.object:int_pos(pos)
--        self.object.yaw = self.object.yaw + 1;

        self.object:int_pos({
            x = self.object.pos.x + 0.08 * math.sin(math.rad(self.object.yaw)),
            y = self.object.pos.y,
            z = self.object.pos.z + 0.08 * math.cos(math.rad(self.object.yaw))})
        self.object:int_yaw(self.object.yaw + 2)
    end
})

if not zepha.is_server() then
    zepha.add_entity("default:test", {x = 10, y = 35, z = 0})
end

zepha.register_entity("default:dropped_item", {
    display = "gameobject",
    display_object = "default:stone",

    on_create = function(self, static)
        if static == nil then static = {} end

        if (static.object) then self.object:set_display_type("gameobject", static.object) end

        self.speed = static.speed or 20
        self.velocityY = static.velocityY or -2.5
        self.time = static.time or 0

        self.object:set_scale(1)
        if not zepha.registered_blocks[static.object] then
            self.object:int_scale(1/2)
        else
            self.object:int_scale(1/3)
        end
    end,
    on_update = function(self, delta)
        self.object:int_yaw(self.object.yaw + self.speed)
        if self.speed > 4 then self.speed = self.speed * 0.92 end

        if (not collides(self.object)) then
            self.velocityY = math.min(self.velocityY + 0.5, 8)
        end

        local v = 1
        while (not collides(self.object) and v <= math.abs(self.velocityY)) do
            local interval = 0
            if self.velocityY < 0 then interval = 1/16 else interval = -1/16 end
            self.object:int_pos({x = self.object.pos.x, y = self.object.pos.y + interval, z = self.object.pos.z})
            v = v + 0.25
        end
        self.object:int_visual_offset({x = 0, y = math.sin(self.time * 2) / 8, z = 0})
        if collides(self.object) then
            self.velocityY = 0
            self.time = self.time + delta
        end

        if self.time > 5 then
            zepha.remove_entity(self)
        end
    end,
    on_destroy = function(self)
        printe("Dropped Item Despawned!")
    end,
    on_serialize = function(self)
        return {
            velocityY = self.velocityY,
            time = self.time,
            speed = self.speed
        }
    end
})