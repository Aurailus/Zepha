if zepha.client then return end

zepha.register_on("new_player", function(p)
    local inv = p:get_inventory()
    inv:add_list("hot_wheel_1", 5, 5)
    inv:add_list("hot_wheel_2", 5, 5)
    inv:add_list("hot_wheel_3", 5, 5)
    inv:add_list("hot_wheel_4", 5, 5)
    inv:add_list("hot_wheel_5", 5, 5)
    inv:add_list("hot_wheel_6", 5, 5)
end)