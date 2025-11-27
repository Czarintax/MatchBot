# Include CS SDK
CSSDK 		= include/cssdk
# Include Metamod SDK
METAMOD 	= include/metamod
# Release file output name
NAME 		= matchbot
# Release directory
BIN_DIR 	= Release
# G++
COMPILER 	= g++
# Source files
SOURCES 	= $(wildcard *.cpp)
# Object files
OBJECTS 	= $(SOURCES:%.cpp=$(BIN_DIR)/%.o)
# Linker Libs
LINKER 		= -static-libgcc -static-libstdc++
# C flags
CFLAGS 		= -std=gnu++11 -Wall -Wno-switch -Wno-unknown-pragmas -Wno-strict-aliasing -shared -m32 -mtune=generic -msse3 -pipe -O2 -fPIC -DNDEBUG -Dlinux -D__linux__ -DGLIBCXX_USE_CXX11_ABI=0 -U_FORTIFY_SOURCE
# Include directory
INCLUDE 	= -I. -I$(CSSDK)/common -I$(CSSDK)/dlls -I$(CSSDK)/engine -I$(CSSDK)/game_shared -I$(CSSDK)/pm_shared -I$(CSSDK)/public -I$(METAMOD)

all:
	mkdir -p $(BIN_DIR)
	$(MAKE) $(NAME)

$(NAME): $(OBJECTS)
	$(COMPILER) $(CFLAGS) $(OBJECTS) $(LINKER) -o $(BIN_DIR)/$(NAME)_mm.so

$(BIN_DIR)/%.o: %.cpp
	$(COMPILER) $(INCLUDE) $(CFLAGS) -o $@ -c $

check:
	cppcheck $(INCLUDE) --quiet --max-configs=100 -DNDEBUG -Dlinux -D__linux__ -DGLIBCXX_USE_CXX11_ABI=0 -U_FORTIFY_SOURCE .

default: all

clean:
	rm -rf $(BIN_DIR)/*.o
	rm -rf $(BIN_DIR)/*.so
