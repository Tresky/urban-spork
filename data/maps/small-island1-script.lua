map_name = "Test Map"

local hero = nil
local EventManager = nil
local camera_zone = nil

function Load()
  local map = rpg_map_mode.MapMode.CurrentInstance()
  EventManager = map:GetEventSupervisor();

  CreateCharacters();

  map:SetCamera(hero, 0);

  camera_zone = rpg_map_mode.CameraZone.Create(256, 0, 320, 320);
  rpg_map_mode.MapTransitionEvent.Create("transition to new map", "small-island", "small-island1");
end

function Update()
  if (camera_zone:IsCameraInside()) then
    -- print "Event Triggered"
    EventManager:LaunchEventById("transition to new map");
  end
end

function CreateCharacters()
  hero = rpg_map_mode.MapSprite.Create(0);

  hero:SetPosition(144, 144);
  hero:SetDimensions(32, 32);
  hero:LoadAnimations("data/entities/actor0-walking.lua");

  enemy0 = rpg_map_mode.EnemySprite.Create();
  enemy0:SetPosition(154, 230);
  enemy0:SetDimensions(32, 32);
  enemy0:LoadAnimations("data/entities/enemy0-walking.lua");
end
