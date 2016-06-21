map_name = "Test Map"

local hero = nil
local EventManager = nil
local camera_zone = nil

local DIRECTION_INVALID = 0;
local DIRECTION_NORTH = 1;
local DIRECTION_NORTHWEST = 2;
local DIRECTION_NORTHEAST = 3;
local DIRECTION_SOUTH = 4;
local DIRECTION_SOUTHWEST = 5;
local DIRECTION_SOUTHEAST = 6;
local DIRECTION_EAST = 7;
local DIRECTION_WEST = 8;

function Load()
  local map = rpg_map_mode.MapMode.CurrentInstance()
  EventManager = map:GetEventSupervisor();

  CreateCharacters();

  map:SetCamera(hero, 0);

  camera_zone = rpg_map_mode.CameraZone.Create(32, 0, 63, 320);
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

  if (rpg_global.GlobalManager:GetPreviousLocation() == "small-island") then
    hero:SetPosition(64, 144);
    hero:SetDirection(DIRECTION_EAST);
  else
    hero:SetPosition(144, 144);
  end
  hero:SetDimensions(32, 32);
  hero:LoadAnimations("data/entities/actor0-walking.lua");

  enemy0 = rpg_map_mode.EnemySprite.Create();
  enemy0:SetPosition(154, 230);
  enemy0:SetDimensions(32, 32);
  enemy0:LoadAnimations("data/entities/enemy0-walking.lua");
end
