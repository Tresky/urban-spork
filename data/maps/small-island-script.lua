map_name = "Test Map"

local hero = nil;
local EventManager = nil;
local camera_zone = nil;

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
  local map = rpg_map_mode.MapMode.CurrentInstance();
  EventManager = map:GetEventSupervisor();

  CreateCharacters();

  map:SetCamera(hero, 0);

  camera_zone = rpg_map_mode.CameraZone.Create(289, 0, 320, 320);
  rpg_map_mode.MapTransitionEvent.Create("transition to new map", "small-island1", "small-island");
end

function Update()
  if (camera_zone:IsCameraInside()) then
    rpg_global.GlobalManager:SetPreviousPosition(hero:GetXPosition(), hero:GetYPosition());
    EventManager:LaunchEventById("transition to new map");
  end
end

function CreateCharacters()
  hero = rpg_map_mode.MapSprite.Create(0);
  hero:SetDimensions(32, 32);
  hero:SetMovementSpeed(6.0);

  if (rpg_global.GlobalManager:GetPreviousLocation() == "small-island1") then
    hero:SetPosition(256, rpg_global.GlobalManager:GetPreviousY());
    hero:SetDirection(DIRECTION_WEST);
  else
    hero:SetPosition(144, 144);
  end
  hero:LoadAnimations("data/entities/actor0-walking.lua");
  hero:LoadAnimations("data/entities/actor0-attacking.lua");

  enemy0 = rpg_map_mode.EnemySprite.Create();
  enemy0:SetDimensions(32, 32);
  enemy0:SetMovementSpeed(7.0);
  enemy0:SetCenterPosition(256, 66);
  enemy0:LoadAnimations("data/entities/enemy0-walking.lua");
  enemy0:AddWayPoint(256, 66);
  enemy0:AddWayPoint(256, 240);
end
