CFLAGS := g++ -Wall -MD -MMD
SRC_DIR := src/
LIB_DIR := libr/
SOURCE_DIRS := . my_simple_computer my_term UI

#OBJ_DIR := obj/
#BIN_DIR := bin/

VPATH := $(SOURCE_DIRS)

.PHONY : all
all: cmpLibs

.PHONY: cmpLibs
cmpLibs:
	ar rc $(SRC_DIR)$(LIB_DIR)libUI.a

