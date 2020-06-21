-- Register the inventory menu and keybind
zepha.register_keybind("zeus:inventory:open_inventory", {
    description = "Open Inventory",
    default = zepha.keys.e,
    on_press = function()
        if zepha.player.menu_state == "" do
            zepha.player:open_menu([[
                body[body]
                    background: #0003

                    rect[inventory]
                        position: 50% 50%
                        position_anchor: 50% 32%
                        size: 218px 160px

                        rect[inv_background]
                            position: 0px 50px
                            size: 218px 100px
                            padding: 20px 10px 8px 10px
                            background: zeus:inventory:inventory

                            inventory
                                source: current_player
                                list: main
                                position: 1px 1px
                                slot_spacing: 2px 2px
                            end
                        end

                        rect[chest_background]
                            position: 0px -48px
                            size: 218px 100px
                            padding: 20px 10px 8px 10px
                            background: zeus:inventory:chest

                            inventory
                                source: current_player
                                list: main
                                position: 1px 1px
                                slot_spacing: 2px 2px
                            end
                        end
                    end
                end
            ]])
        else zepha.player:close_menu() end
    }
})