DEBUG=0
FULLMENUTRANSPARANCY = 0
SRC_DIR = src
SRC_SUBDIR = games
OBJ_DIR = obj
DAT_DIR = retrotimefs
EXE=retrotime

SRC=$(wildcard $(SRC_DIR)/*.cpp $(foreach fd, $(SRC_SUBDIR), $(SRC_DIR)/$(fd)/*.cpp))
OBJS=$(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)


CXX ?= g++
SDL2CONFIG ?= sdl2-config
DEFINES ?= 
DESTDIR ?=
PREFIX ?= /usr
OPT_LEVEL ?= -O2
CPPFLAGS ?= -Wall -Wextra -std=c++11 `$(SDL2CONFIG) --cflags`
LDFLAGS ?= -g
LDLIBS ?= `$(SDL2CONFIG) --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2 -lSDL2_gfx -lstdc++

DEFINESADD = 
ifneq ($(FULLMENUTRANSPARANCY), 0)
DEFINESADD += -DFULLMENUTRANSPARANCY
endif

FINALDEFINES = $(DEFINES) $(DEFINESADD)

ifeq ($(DEBUG), 1)
ifeq ($(OS),Windows_NT)
LDLIBS += -mconsole
endif
CPPFLAGS += -g
OPT_LEVEL =
endif

#MINGW does not have X11 and does not require it
#dont know about cygwin
ifneq ($(OS),Windows_NT)
ifeq ($(NOX11),)
LDLIBS += -lX11
endif
endif

GAMEDIR = $(DESTDIR)$(PREFIX)/games/retrotime
DESKTOPDIR = $(DESTDIR)$(PREFIX)/share/applications
ICONDIR = $(DESTDIR)$(PREFIX)/share/icons/


.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(OPT_LEVEL) $(LDFLAGS) $(TARGET_ARCH) $(FINALDEFINES) $^ $(LDLIBS) -o $@ 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)  
	$(CXX) $(OPT_LEVEL) $(CPPFLAGS) $(CXXFLAGS) $(FINALDEFINES) -c $< -o $@

$(OBJ_DIR): $(SRC_SUBDIR)
	mkdir -p $@

$(SRC_SUBDIR):
	mkdir -p $(OBJ_DIR)/$@

clean:
	$(RM) -rv *~ $(OBJ_DIR) $(EXE)

install: all
	install -d $(GAMEDIR)
	install -d $(DESKTOPDIR)
	install -d $(ICONDIR)

	install -m 0755 $(EXE) $(GAMEDIR)
	sed -i '6c\Exec=$(GAMEDIR)/$(EXE)' $(DAT_DIR)/data/io.github.joyrider3774.retrotime.desktop
	install -m 0644 $(DAT_DIR)/data/io.github.joyrider3774.retrotime.desktop $(DESKTOPDIR) 
	install -m 0644 $(DAT_DIR)/data/io.github.joyrider3774.retrotime.svg $(ICONDIR)
	\cp -rf $(DAT_DIR) $(GAMEDIR)

uninstall:
	$(RM) -rf $(GAMEDIR)
	$(RM) -f $(DESKTOPDIR)/io.github.joyrider3774.retrotime.desktop
	$(RM) -f $(ICONDIR)/io.github.joyrider3774.retrotime.svg