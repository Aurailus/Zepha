--local noise = {
--    module = "add",
--    sources = {
--        { -- Elevation
--            module = "scale_bias",
--            source = {
--                module = "perlin",
--                frequency = 0.02,
--                octaves = 8
--            },
--            scale = 250,
--            bias = 32
--        },
--        { -- Features
--            module = "scale_bias",
--            source = {
--                module = "perlin",
--                frequency = 0.8,
--                octaves = 3,
--            },
--            scale = 30,
--            bias = 6
--        }
--    }
--}
--
--zepha.register_biome("zeus:mapgen:desert", {
--    environment = {
--        temperature = 40/100,
--        humidity = 20/100,
--        roughness = 10/100
--    },
--    blocks = {
--        top = "zeus:default:sand",
--        soil = "zeus:default:sand",
--        rock = "zeus:default:sandstone"
--    },
--    biome_tint = "#e6fa61",
--    noise = noise
--})