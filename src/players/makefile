# src/players makefile

include $(MAKE_VARS)

NAME = libPlayers.so

SOURCE_FILES = \
   RandomAi.cpp \
   LearningAi.cpp
   
LIBS = \
   -lCards \
   -lAssets \
   -lGameBoards

all: $(SOURCE_FILES)
	g++ $(COMP_FLAGS) -fPIC -shared $(INC_DIRS) $(SOURCE_FILES) $(LIB_DIRS) $(LIBS) -o $(BLD_DIR)/$(NAME)