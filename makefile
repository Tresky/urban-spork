# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall

# Relative directories
ODIR = obj

#Libraries to link to
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -llua -L/usr/local/lib
INCS = -I/usr/local/include


#### OBJECT FILES ####
# Engine Objects
_ENG_OBJ = system.o mode_manager.o video_manager.o script.o script_read.o resources.o image.o input.o lua_bindings.o
ENG_OBJ = $(patsubst %, $(ODIR)/%, $(_ENG_OBJ))

# Global Objects
_GLOB_OBJ = global.o global_actors.o
GLOB_OBJ = $(patsubst %, $(ODIR)/%, $(_GLOB_OBJ))

# Mode Objects
_MOD_OBJ = map_utils.o map_mode.o map_tiles.o map_objects.o map_sprites.o
MOD_OBJ = $(patsubst %, $(ODIR)/%, $(_MOD_OBJ))



#### DEPENDENCY HEADER FILES ####
# Engine Headers
_ENG_DEP = system.hpp mode_manager.hpp video_manager.hpp script.hpp script_read.hpp resources.hpp image.hpp input.hpp
ENG_DEP = $(patsubst %, ./src/core/%, $(_ENG_DEP))

# Global Headers
_GLOB_DEP = global.hpp global_actors.hpp
GLOB_DEP = $(patsubst %, ./src/core/global/%, $(_GLOB_DEP))

# Mode Headers
_MOD_DEP = map_utils.hpp map_mode.hpp map_tiles.hpp map_objects.hpp map_sprites.hpp
MOD_DEP = $(patsubst %, ./src/modes/map/%, $(_MOD_DEP))



#### RULES ####
# Links object files and libraries into executable
rpg: $(ENG_OBJ) $(GLOB_OBJ) $(MOD_OBJ) obj/main.o
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS) $(INCS)

# Compiles engine from /src/core/*
$(ODIR)/%.o: ./src/core/%.cpp $(ENG_DEP)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Compiles engine from /src/core/global/*
$(ODIR)/%.o: ./src/core/global/%.cpp $(GLOB_DEP)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Compiles map mode from /src/modes/map/*
$(ODIR)/%.o: ./src/modes/map/%.cpp $(MOD_DEP)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Compiles the main.cpp file and places the object
# file in the obj/ directory.
$(ODIR)/main.o: ./src/main.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)



#### CLEANUP ####
# Ensures that the clean 'rule' isn't mistaken for
# a file name. If a file called 'clean' were made and
# this line wasn't present, the 'clean' file would not
# compile because it has no prerequisites and would be
# considered up-to-date all the time.
#
# NOTE: Rules only activate if the dependant files
# (right side of colon) have changed since the last time
# the make was run.
.PHONY: clean

# Ran independantly.
# Deletes all object files and the executable.
clean:
	rm -f $(ODIR)/*.o #*~ core ./src/*~
	rm -f rpg
