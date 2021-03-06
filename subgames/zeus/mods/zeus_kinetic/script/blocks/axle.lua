zepha.register_block('zeus:kinetic:axle_0', {
    name = "Axle",
    model = "zeus:kinetic:axle_0",
    textures = {"zeus:kinetic:axle"},
    culls = false,
    selection_box = {
        {0, 6/16, 6/16, 1, 10/16, 10/16}
    },
    collision_box = {
        {0, 6/16, 6/16, 1, 10/16, 10/16}
    },
    drop = "zeus:kinetic:axle_0",
    on_place = function(pos)
        for i = 0, 9 do
            pos.x = pos.x + 1
            zepha.set_block(pos, "zeus:kinetic:axle_0")
        end
    end,
    on_place_client = function(pos)
        for i = 0, 9 do
            pos.x = pos.x + 1
            zepha.set_block(pos, "zeus:kinetic:axle_0")
        end
    end,
    on_construct = function(pos)
        zepha.after(function()
            zepha.set_block(pos, "zeus:kinetic:axle_1")
        end, 4)
    end
})

zepha.register_block('zeus:kinetic:axle_1', {
    name = "Axle",
    model = "zeus:kinetic:axle_1",
    textures = {"zeus:kinetic:axle"},
    culls = false,
    selection_box = {
        {0, 6/16, 6/16, 1, 10/16, 10/16}
    },
    collision_box = {
        {0, 6/16, 6/16, 1, 10/16, 10/16}
    },
    on_construct = function(pos)
        zepha.after(function()
            zepha.set_block(pos, "zeus:kinetic:axle_2")
        end, 4)
    end
})

zepha.register_block('zeus:kinetic:axle_2', {
    name = "Axle",
    model = "zeus:kinetic:axle_2",
    textures = {"zeus:kinetic:axle"},
    culls = false,
    selection_box = {
        {0, 6/16, 6/16, 1, 10/16, 10/16}
    },
    collision_box = {
        {0, 6/16, 6/16, 1, 10/16, 10/16}
    },
    on_construct = function(pos)
        zepha.after(function()
            zepha.set_block(pos, "zeus:kinetic:axle_3")
        end, 4)
    end
})

zepha.register_block('zeus:kinetic:axle_3', {
    name = "Axle",
    model = "zeus:kinetic:axle_3",
    textures = {"zeus:kinetic:axle"},
    culls = false,
    selection_box = {
        {0, 6/16, 6/16, 1, 10/16, 10/16}
    },
    collision_box = {
        {0, 6/16, 6/16, 1, 10/16, 10/16}
    },
    on_construct = function(pos)
        zepha.after(function()
            zepha.set_block(pos, "zeus:kinetic:axle_0")
        end, 4)
    end
})