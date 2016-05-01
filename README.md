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
