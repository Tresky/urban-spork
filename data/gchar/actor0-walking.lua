print 'lua stuff'

animations = {}
animations.filepath = "data/gchar/actor0.png"

-- Walking Down Animation
animations[0] = {
  name = "walk-south",
  width = 32,
  height = 32,
  num_frames = 3,
  frame_rects = [
    {x = 0, y = 0},
    {x = 32, y = 32},
    {x = 64, y = 64}
  ]
}

animation[1] = {
  name = "walk-west",
  width = 32,
  height = 32,
  num_frames = 3,
  frame_rects = [
    {x = 0, y = 32},
    {x = 32, y = 32},
    {x = 64, y = 32}
  ]
}

animation[1] = {
  name = "walk-east",
  width = 32,
  height = 32,
  num_frames = 3,
  frame_rects = [
    {x = 0, y = 64},
    {x = 32, y = 64},
    {x = 64, y = 64}
  ]
}

animation[1] = {
  name = "walk-north",
  width = 32,
  height = 32,
  num_frames = 3,
  frame_rects = [
    {x = 0, y = 96},
    {x = 32, y = 96},
    {x = 64, y = 96}
  ]
}
