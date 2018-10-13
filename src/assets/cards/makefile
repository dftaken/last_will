# src/assets/cards makefile

include $(MAKE_VARS)

NAME = libCards.so

SOURCE_FILES = \
   CardAttributes.cpp \
   Card.cpp \
   CardLibrary.cpp

all: $(SOURCE_FILES)
	g++ $(COMP_FLAGS) -fPIC -shared $(INC_DIRS) $(SOURCE_FILES) -o $(BLD_DIR)/$(NAME)