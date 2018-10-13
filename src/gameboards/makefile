# src/gameboards makefile

include $(MAKE_VARS)

NAME = libGameBoards.so

SOURCE_FILES = \
   GameBoard.cpp \
   GamePlayer.cpp
   
LIBS = \
   -lCards \
   -lAssets

all: $(SOURCE_FILES)
	g++ $(COMP_FLAGS) -fPIC -shared $(INC_DIRS) $(SOURCE_FILES) $(LIB_DIRS) $(LIBS) -o $(BLD_DIR)/$(NAME)