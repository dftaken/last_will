# src/makefile

include $(MAKE_VARS)

SOURCE_FILES = \
   main.cpp \
   GameMaster.cpp \
   Logger.cpp
   
LIBS = \
   -lAssets \
   -lCards \
   -lGameBoards \
   -lPlayers \
   -lDatabase \
   -lpthread \
   -lSysUtils

Assets:
	echo "Building assets"
	$(MAKE) -C assets MAKE_VARS=$(MAKE_VARS) all

GameBoards:
	echo "Building gameboards"
	$(MAKE) -C gameboards MAKE_VARS=$(MAKE_VARS) all

Players:
	echo "Building players"
	$(MAKE) -C players MAKE_VARS=$(MAKE_VARS) all

Database:
	echo "Building database"
	$(MAKE) -C database MAKE_VARS=$(MAKE_VARS) all

all: Assets GameBoards Players Database
	g++ $(COMP_FLAGS) -I. -I$(DEV_ROOT) $(SOURCE_FILES) $(LIB_DIRS) $(LIBS) -o $(BLD_DIR)/$(OUT_NAME)
	g++ $(COMP_FLAGS) -I. -I$(DEV_ROOT) Analyzer.cpp $(LIB_DIRS) -lDatabase -o $(BLD_DIR)/lw_analyzer
	g++ $(COMP_FLAGS) -I. -I$(DEV_ROOT) CardAnalysis.cpp $(LIB_DIRS) -lCards -o $(BLD_DIR)/lw_card_analysis
