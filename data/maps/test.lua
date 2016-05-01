map_data = {}
map_data.name = "Test Map"
map_data.num_cols = 6
map_data.num_rows = 9

map_data.tilesets = {}
map_data.tilesets.count = 1
map_data.tilesets[0] = "data/maps/tileset.lua"

map_data.collision = {}
map_data.collision[1] = {1, 1, 1, 1, 1, 1}
map_data.collision[2] = {1, 0, 0, 0, 0, 1}
map_data.collision[3] = {1, 0, 0, 0, 0, 1}
map_data.collision[4] = {1, 0, 0, 0, 0, 1}
map_data.collision[5] = {1, 0, 0, 0, 0, 1}
map_data.collision[6] = {1, 0, 0, 0, 0, 1}
map_data.collision[7] = {1, 0, 0, 0, 0, 1}
map_data.collision[8] = {1, 0, 0, 0, 0, 1}
map_data.collision[9] = {1, 1, 1, 1, 1, 1}

map_data.layers = {}
map_data.layers.num_layers = 2
map_data.layers[0] = {}
map_data.layers[0].type = "ground"
map_data.layers[0].name = "Background"
map_data.layers[0][1] = {0,  1,  1,  1,  1,  2}
map_data.layers[0][2] = {16, 17, 17, 17, 17, 18}
map_data.layers[0][3] = {16, 17, 17, 17, 17, 18}
map_data.layers[0][4] = {16, 17, 17, 17, 17, 18}
map_data.layers[0][5] = {16, 17, 17, 17, 17, 18}
map_data.layers[0][6] = {16, 17, 17, 17, 17, 18}
map_data.layers[0][7] = {16, 17, 17, 17, 17, 18}
map_data.layers[0][8] = {16, 17, 17, 17, 17, 18}
map_data.layers[0][9] = {32, 33, 33, 33, 33, 34}

map_data.layers[1] = {}
map_data.layers[1].type = "wall"
map_data.layers[1].name = "Foreground"
map_data.layers[1][1] = {-1, -1, -1, -1, -1, -1}
map_data.layers[1][2] = {-1, -1, -1, -1, -1, -1}
map_data.layers[1][3] = {-1, -1, -1, -1, -1, -1}
map_data.layers[1][4] = {-1, -1, -1, -1, -1, -1}
map_data.layers[1][5] = {-1, -1, -1, -1, -1, -1}
map_data.layers[1][6] = {-1, -1, -1, -1, -1, -1}
map_data.layers[1][7] = {-1, -1, -1, -1, 165, 164}
map_data.layers[1][8] = {-1, -1, -1, -1, 166, -1}
map_data.layers[1][9] = {-1, -1, -1, -1, -1, -1}
