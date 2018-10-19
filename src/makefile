# src/makefile

include $(MAKE_VARS)

SOURCE_FILES = \
   main.cpp \
   GameMaster.cpp \
   Logger.cpp \
   Indexer.cpp \
   Database.cpp
   
LIBS = \
   -lAssets \
   -lCards \
   -lGameBoards \
   -lPlayers

Assets:
	echo "Building assets"
	$(MAKE) -C assets MAKE_VARS=$(MAKE_VARS) all

GameBoards:
	echo "Building gameboards"
	$(MAKE) -C gameboards MAKE_VARS=$(MAKE_VARS) all

Players:
	echo "Building players"
	$(MAKE) -C players MAKE_VARS=$(MAKE_VARS) all

all: Assets GameBoards Players
	g++ $(COMP_FLAGS) -I. -I$(DEV_ROOT) $(SOURCE_FILES) $(LIB_DIRS) $(LIBS) -o $(BLD_DIR)/$(OUT_NAME)
