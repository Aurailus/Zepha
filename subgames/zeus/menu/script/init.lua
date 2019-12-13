zepha.set_gui([[
    body
        background: asset(zeus_background)

        rect[sidebar]
            position: 20% 0
            position_anchor: 50% 0
            size: 102px 100%
            background: #0135

            rect[logo]
                position: 8px 8px
                size: 86px 30px
                background: asset(zeus_logo)
            end

            button[buttonPlay]
                position: 6px 50px
                size: 90px 20px
                background: asset(crop(0, 0, 90, 20, zeus_button))
                background_hover: asset(crop(0, 20, 90, 20, zeus_button))
                content: "Local Play"
            end

            button[buttonServers]
                position: 6px 74px
                size: 90px 20px
                background: asset(crop(0, 0, 90, 20, zeus_button))
                background_hover: asset(crop(0, 20, 90, 20, zeus_button))
                content: "Browse Servers"
            end
        end
    end
]], {
    buttonPlay = function()
        zepha.start_game()
    end,
    buttonServers = function()
        zepha.server_select()
    end
})