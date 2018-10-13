# src/assets makefile

include $(MAKE_VARS)

NAME = libAssets.so

SOURCE_FILES = \
   Action.cpp \
   PlayerBoard.cpp \
   PlayerState.cpp \
   Deck.cpp \
   PropertyMarket.cpp \
   BoardSpot.cpp \
   CardSpot.cpp \
   PlayerHand.cpp
   
LIBS = \
   -lCards \
   -lSysUtils

Cards:
	echo "Building assets"
	$(MAKE) -C cards MAKE_VARS=$(MAKE_VARS) all

all: $(SOURCE_FILES) Cards
	g++ $(COMP_FLAGS) -fPIC -shared $(INC_DIRS) $(SOURCE_FILES) $(LIB_DIRS) $(LIBS) -o $(BLD_DIR)/$(NAME)