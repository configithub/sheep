# -*- mode: Makefile; -*-
# -----------------------------------------
# project sheep


export PATH := /opt/wx/2.8/bin:$(PATH)
export LD_LIBRARY_PATH := /opt/wx/2.8/lib:$(LD_LIBRARY_PATH)

_WX = /home/gr/projects/gui/codeblocks/wx
_WX.LIB = $(_WX)/lib
_WX.INCLUDE = $(_WX)/include

_CB = /home/gr/projects/gui/codeblocks/cb/src
_CB.INCLUDE = $(_CB)/include
_CB.LIB = $(_CB)/devel



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
Release_target.OBJ = src/Effect.o src/Bomb.o src/CAnimation.o src/CApp.o src/CApp_OnCleanup.o src/CApp_OnEvent.o src/CApp_OnInit.o src/CApp_OnLoop.o src/CApp_OnRender.o src/CArea.o src/CCamera.o src/CEntity.o src/CEntityCol.o src/CEvent.o src/CFPS.o src/CFollower.o src/CMap.o src/CSurface.o src/CTile.o src/Level.o src/MultitouchEvent.o 
DEP_FILES += src/Effect.d src/Bomb.d src/CAnimation.d src/CApp.d src/CApp_OnCleanup.d src/CApp_OnEvent.d src/CApp_OnInit.d src/CApp_OnLoop.d src/CApp_OnRender.d src/CArea.d src/CCamera.d src/CEntity.d src/CEntityCol.d src/CEvent.d src/CFPS.d src/CFollower.d src/CMap.d src/CSurface.d src/CTile.d src/Level.d src/MultitouchEvent.d 
clean.OBJ += $(Release_target.BIN) $(Release_target.OBJ)

Release_target : Release_target.before $(Release_target.BIN) Release_target.after_always
Release_target : CFLAGS +=  
Release_target : INCLUDES += -I/usr/lib -I/usr/include/SDL 
Release_target : RCFLAGS += 
Release_target : LDFLAGS +=  -lSDLmain -lSDL -lSDL_image -lSDL_gfx -L/usr/lib   
Release_target : T_LDLIBS = /usr/lib/libSDLmain.a /usr/lib/libSDL.a /usr/lib/libSDL_image.a /usr/lib/libSDL_gfx.a 

Release_target.before :
	
	
Release_target.after_always : $(Release_target.BIN)
	
$(Release_target.BIN) : $(Release_target.OBJ)
	$(LINK_con)
	

# -----------------------------------------
ifdef MAKE_DEP
-include $(DEP_FILES)
endif
