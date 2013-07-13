# -*- mode: Makefile; -*-
# -----------------------------------------
# project sheep


export PATH := $(PATH)
export LD_LIBRARY_PATH := $(LD_LIBRARY_PATH)



CFLAGS_C = $(filter-out -include "sdk.h",$(CFLAGS))

# -----------------------------------------

# MAKE_DEP = -MMD -MT $@ -MF $(@:.o=.d)

CFLAGS = -g
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
Release_target.OBJ = src/Switch.o src/Effect.o src/Bomb.o src/CAnimation.o src/CApp.o src/CApp_OnCleanup.o src/CApp_OnEvent.o src/CApp_OnInit.o src/CApp_OnLoop.o src/CApp_OnRender.o src/Area.o src/CCamera.o src/CEntity.o src/CEntityCol.o src/CEvent.o src/CFPS.o src/CFollower.o src/CMap.o src/CSurface.o src/CTile.o src/MultitouchEvent.o src/Behavior.o src/Saw.o src/Door.o
DEP_FILES += src/Switch.d src/Effect.d src/Bomb.d src/CAnimation.d src/CApp.d src/CApp_OnCleanup.d src/CApp_OnEvent.d src/CApp_OnInit.d src/CApp_OnLoop.d src/CApp_OnRender.d src/Area.d src/CCamera.d src/CEntity.d src/CEntityCol.d src/CEvent.d src/CFPS.d src/CFollower.d src/CMap.d src/CSurface.d src/CTile.d src/MultitouchEvent.d src/Behavior.d src/Saw.d src/Door.d
clean.OBJ += $(Release_target.BIN) $(Release_target.OBJ)

Release_target : Release_target.before $(Release_target.BIN) Release_target.after_always
Release_target : CFLAGS +=  
Release_target : INCLUDES += -I/usr/lib -I/usr/include/SDL 
Release_target : RCFLAGS += 
Release_target : LDFLAGS +=  -lSDLmain -lSDL -lSDL_image -L/usr/lib/i386-linux-gnu   
Release_target : LDFLAGS +=  -lGL -L/usr/lib/i386-linux-gnu

Release_target.before :
	
	
Release_target.after_always : $(Release_target.BIN)
	
$(Release_target.BIN) : $(Release_target.OBJ)
	$(LINK_con)
	

# -----------------------------------------
ifdef MAKE_DEP
-include $(DEP_FILES)
endif
