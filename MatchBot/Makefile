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
# Linker Libs
LINKER 		= -static-libgcc -static-libstdc++
# C flags
CFLAGS 		= -std=gnu++11 -shared -m32 -mtune=generic -msse3 -pipe -O2 -fno-strict-aliasing -fvisibility=hidden -DNDEBUG -Dlinux -D__linux__ -DGLIBCXX_USE_CXX11_ABI=1 -U_FORTIFY_SOURCE
# Include directory
INCLUDE 	= -I. -I$(CSSDK)/common -I$(CSSDK)/dlls -I$(CSSDK)/engine -I$(CSSDK)/game_shared -I$(CSSDK)/pm_shared -I$(CSSDK)/public -I$(METAMOD)
# Object files
OBJECTS 	= $(SOURCES:%.cpp=$(BIN_DIR)/%.o)

all:
	mkdir -p $(BIN_DIR)
	$(MAKE) $(NAME)
	strip -x $(BIN_DIR)/$(NAME)_mm.so

$(NAME): $(OBJECTS)
	$(COMPILER) $(CFLAGS) $(OBJECTS) $(LINKER) -o $(BIN_DIR)/$(NAME)_mm.so

$(BIN_DIR)/%.o: %.cpp
	$(COMPILER) $(INCLUDE) $(CFLAGS) -o $@ -c $

check:
	cppcheck $(INCLUDE) --quiet --max-configs=100 --std=gnu++11 -DNDEBUG -Dlinux -D__linux__ -DGLIBCXX_USE_CXX11_ABI=1 -U_FORTIFY_SOURCE .

default: all

clean:
	rm -rf $(BIN_DIR)/*.o
	rm -rf $(BIN_DIR)/*.so
