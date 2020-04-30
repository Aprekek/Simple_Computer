CC := g++
FLAGS := -Wall -MD -MMD
D_DIR := f_d
OBJ_DIR := f_obj
LIB_DIR := a_libr
OBJ_FILES := $(wildcard $(OBJ_DIR)/*.o)

.PHONY : all
all: compile makeLib replace
compile: $(patsubst %.cpp, %.o, $(wildcard *.cpp))

%.o: %.cpp
	$(CC) $(FLAGS) -c $<

.PHONY : replace
replace:
	mv *.d $(D_DIR) && mv *.o $(OBJ_DIR)

.PHONY : makeLib
makeLib:
	ar rvc ../$(LIB_DIR)/libalu_cu.a *.o
	ranlib ../$(LIB_DIR)/libalu_cu.a
