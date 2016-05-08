# CPP RPG

### To-Do
 - ObjectSupervisor (maintains collision grid and objects)
 - Get GlobalCharacter walking around
 - Get collisions working


Relationship between ScriptManager and ResourceManager
 - ScriptManager defines methods for loading specific types of objects from scripts.
   - LoadCharacter(lua), LoadObject(lua), LoadSkill(lua), etc
   - Whenever ScriptManager loads from scripts, the resources are stored in the requesting
     object and the ResourceManager.

 - ResourceManager is a holding bin for sprites, sounds, backdrops, etc.
   - All resources are accessible via the object id (int or string?)

 - Need to load objects and collision data from Script

 ### Milestones
 - ~Collision detection
 - ~ScriptRead class~
 - ~Character animations~
 - Redo collision detection
 - Map functional
 - Basic fighting w/ enemies
 - Basic UI (health, mp, etc)

Notes:
 - Detach position from GlobalCharacter. Camera should be independent from Character.
 - Store PArty member in GlobalEngine

 ObjectSupervisor contain pointer to MapSprite for character
 MapMode contains VirtualSprite for camera

 In Lua set camera to Sprite created in Lua. For pre-tests, we can abstract the Lua functions into test C++.
