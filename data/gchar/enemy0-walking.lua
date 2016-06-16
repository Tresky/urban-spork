animations = {}
animations.filepath = "data/gchar/enemy0.png"
animations.num_animations = 8

-- Walking Down Animation
animations[0] = {
  name = "walk-south",
  width = 32,
  height = 32,
  num_frames = 3,
  frame_time = 200,
  frame_rects = {
    {x = 0, y = 0},
    {x = 32, y = 0},
    {x = 64, y = 0}
  }
}

animations[1] = {
  name = "walk-west",
  width = 32,
  height = 32,
  num_frames = 3,
  frame_time = 200,
  frame_rects = {
    {x = 0, y = 32},
    {x = 32, y = 32},
    {x = 64, y = 32}
  }
}

animations[2] = {
  name = "walk-east",
  width = 32,
  height = 32,
  num_frames = 3,
  frame_time = 200,
  frame_rects = {
    {x = 0, y = 64},
    {x = 32, y = 64},
    {x = 64, y = 64}
  }
}

animations[3] = {
  name = "walk-north",
  width = 32,
  height = 32,
  num_frames = 3,
  frame_time = 200,
  frame_rects = {
    {x = 0, y = 96},
    {x = 32, y = 96},
    {x = 64, y = 96}
  }
}

animations[4] = {
  name = "idle-south",
  width = 32,
  height = 32,
  num_frames = 2,
  frame_time = 400,
  frame_rects = {
    {x = 0, y = 0},
    {x = 32, y = 0}
  }
}

animations[5] = {
  name = "idle-west",
  width = 32,
  height = 32,
  num_frames = 2,
  frame_time = 400,
  frame_rects = {
    {x = 0, y = 32},
    {x = 32, y = 32}
  }
}

animations[6] = {
  name = "idle-east",
  width = 32,
  height = 32,
  num_frames = 2,
  frame_time = 400,
  frame_rects = {
    {x = 0, y = 64},
    {x = 32, y = 64}
  }
}

animations[7] = {
  name = "idle-north",
  width = 32,
  height = 32,
  num_frames = 2,
  frame_time = 400,
  frame_rects = {
    {x = 0, y = 96},
    {x = 32, y = 96}
  }
}
