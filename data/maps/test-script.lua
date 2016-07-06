map_name = "Test Map"

local hero = nil

local EventManager = nil

local camera_zone = nil

function Load()
  local map = rpg_map_mode.MapMode.CurrentInstance()
  EventManager = map:GetEventSupervisor();

  CreateCharacters();

  map:SetCamera(hero, 0);

  camera_zone = rpg_map_mode.CameraZone.Create(320, 32, 322, 128);
  rpg_map_mode.MapTransitionEvent.Create("transition to new map", "small-island", "test");
end

function Update()
  if (camera_zone:IsCameraInside()) then
    -- print "Event Triggered"
    EventManager:LaunchEventById("transition to new map");
  end
end

function CreateCharacters()
  hero = rpg_map_mode.MapSprite.Create(0);

  hero:SetPosition(64, 64);
  hero:SetDimensions(32, 32);
  hero:LoadAnimations("data/entities/actor0-walking.lua");

  enemy0 = rpg_map_mode.EnemySprite.Create();
  enemy0:SetPosition(400, 280);
  enemy0:SetDimensions(32, 32);
  enemy0:LoadAnimations("data/entities/enemy0-walking.lua");

  enemy1 = rpg_map_mode.EnemySprite.Create();
  enemy1:SetPosition(280, 400);
  enemy1:SetDimensions(32, 32);
  enemy1:LoadAnimations("data/entities/enemy0-walking.lua");

  enemy2 = rpg_map_mode.EnemySprite.Create();
  enemy2:SetPosition(500, 500);
  enemy2:SetDimensions(32, 32);
  enemy2:LoadAnimations("data/entities/enemy0-walking.lua");
end
