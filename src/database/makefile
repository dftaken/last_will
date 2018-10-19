# src/players makefile

include $(MAKE_VARS)

NAME = libDatabase.so

SOURCE_FILES = \
  Indexer.cpp \
  Database.cpp
   
LIBS = \
   -lCards \
   -lAssets \
   -lGameBoards

all: $(SOURCE_FILES)
	g++ $(COMP_FLAGS) -fPIC -shared $(INC_DIRS) $(SOURCE_FILES) $(LIB_DIRS) $(LIBS) -o $(BLD_DIR)/$(NAME)