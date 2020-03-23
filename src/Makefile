CC := g++
FLAGS := -Wall
INCL_FL := -I

D_DIR := f_d
LIB_DIR := a_libr/
LIBR := $(addprefix -L, $(LIB_DIR))
LIBS := -lui -lmy_key_board -lbig_chars -lmy_term -ls_computer

DIRS := my_simple_computer my_term big_chars read_key UI
INCLUDE := $(addsuffix /$(D_DIR), $(addprefix $(INCL_FL), $(DIRS)))

.PHONY : all
all: compile

.PHONY :
compile: launcher

launcher: launcher.cpp
	$(CC) $(FLAGS) $(INCLUDE) -o ../launcher $< $(LIBR) $(LIBS)

#include: big_chars/f_d/*.d my_simple_computer/f_d/*.d my_term/f_d/*.d UI/f_d/*.d
