# -*- mode: Makefile; -*-
# -----------------------------------------
# project sheep


export PATH := $(PATH)
export LD_LIBRARY_PATH := $(LD_LIBRARY_PATH)



CFLAGS_C = $(filter-out -include "sdk.h",$(CFLAGS))

# -----------------------------------------

# MAKE_DEP = -MMD -MT $@ -MF $(@:.o=.d)

CFLAGS = -g -O3 -ffast-math
INCLUDES = 
LDFLAGS = 
RCFLAGS = 
LDLIBS = $(T_LDLIBS)  -lstdc++

LINK_exe = gcc -o $@ $^ $(LDFLAGS) $(LDLIBS)
LINK_con = gcc -o $@ $^ $(LDFLAGS) $(LDLIBS)
LINK_dll = gcc -o $@ $^ $(LDFLAGS) $(LDLIBS) -shared
LINK_lib = rm -f $@ && ar rcs $@ $^
COMPILE_c = gcc $(CFLAGS_C) -o $@ -c $< $(MAKEDEP) $(INCLUDES)
COMPILE_cpp = g++ $(CFLAGS) -o $@ -c $< $(MAKEDEP) $(INCLUDES)
COMPILE_rc = windres $(RCFLAGS) -J rc -O coff -i $< -o $@ -I$(dir $<)

%.o : %.c ; $(COMPILE_c)
%.o : %.cpp ; $(COMPILE_cpp)
%.o : %.cxx ; $(COMPILE_cpp)
%.o : %.rc ; $(COMPILE_rc)
.SUFFIXES: .o .d .c .cpp .cxx .rc

all: all.before all.targets all.after

all.before :
	-
all.after : $(FIRST_TARGET)
	
all.targets : Release_target 

clean :
	rm -fv $(clean.OBJ)
	rm -fv $(DEP_FILES)

.PHONY: all clean distclean


# -----------------------------------------
# Release_target

Release_target.BIN = bin/Release/sheep.exe
Release_target.OBJ = src/Switch.o src/Effect.o src/Bomb.o src/Animation.o src/CApp.o src/OnCleanup.o src/OnEvent.o src/OnInit.o src/OnLoop.o src/OnRender.o src/Area.o src/Camera.o src/Entity.o src/EntityCol.o src/Event.o src/FPS.o src/Follower.o src/Map.o src/Texture.o src/Tile.o src/MultitouchEvent.o src/Behavior.o src/Saw.o src/Door.o src/LuaBinds.o src/Gesture.o src/OnGesture.o
DEP_FILES += src/Switch.d src/Effect.d src/Bomb.d src/Animation.d src/CApp.d src/OnCleanup.d src/OnEvent.d src/OnInit.d src/OnLoop.d src/OnRender.d src/Area.d src/Camera.d src/Entity.d src/EntityCol.d src/Event.d src/FPS.d src/Follower.d src/Map.d src/Texture.d src/Tile.d src/MultitouchEvent.d src/Behavior.d src/Saw.d src/Door.d src/LuaBinds.d src/Gesture.d src/OnGesture.d
clean.OBJ += $(Release_target.BIN) $(Release_target.OBJ)

Release_target : Release_target.before $(Release_target.BIN) Release_target.after_always
Release_target : CFLAGS +=  
Release_target : INCLUDES += -I/usr/lib -I/usr/include/SDL -I/usr/include/lua5.1
Release_target : RCFLAGS += 
Release_target : LDFLAGS +=  -lSDLmain -lSDL -lSDL_image -L/usr/lib/i386-linux-gnu   
Release_target : LDFLAGS +=  -lGL -L/usr/lib/i386-linux-gnu   
Release_target : LDFLAGS +=  -llua5.1 -L/usr/lib/i386-linux-gnu

Release_target.before :
	
	
Release_target.after_always : $(Release_target.BIN)
	
$(Release_target.BIN) : $(Release_target.OBJ)
	$(LINK_con)
	

# -----------------------------------------
ifdef MAKE_DEP
-include $(DEP_FILES)
endif
