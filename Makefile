#CFLAGS := g++ -Wall -MD -MMD
SRC_DIR := src/
LIB_DIR := a_libr/

SOURCE_DIRS := my_simple_computer my_term big_chars read_key CU_ALU translators UI 
SOURCE_DIRS := $(addprefix $(SRC_DIR), $(SOURCE_DIRS))

OBJ_DIR := f_obj
OBJ_DIRS := $(addsuffix /$(OBJ_DIR),$(SOURCE_DIRS))

D_DIR := f_d
D_DIRS := $(addsuffix /$(D_DIR), $(SOURCE_DIRS))

SEARCH_WILDCARD := $(wildcard $(addsuffix /*.cpp, $(SOURCE_DIRS)))

VPATH := $(SOURCE_DIRS)


.PHONY : all
all: makeDirs compile

.PHONY : compile
compile: my_simple_computer my_term big_chars read_key CU_ALU translators UI launcher 

.PHONY : my_simple_computer
my_simple_computer:
	cd src/my_simple_computer/ && make all

.PHONY : my_term
my_term:
	cd src/my_term/ && make all

.PHONY : read_key
read_key:
	cd src/read_key/ && make all

.PHONY : CU_ALU
CU_ALU:
	cd src/CU_ALU && make all

.PHONY : UI
UI:	
	cd src/UI/ && make all

.PHONY : big_chars
big_chars:
	cd src/big_chars/ && make all

.PHONY : translators
translators:
	cd src/translators/ && make all

.PHONY : launcher
launcher:
	cd src/ && make all

.PHONY : makeDirs
makeDirs:
	mkdir -p $(D_DIRS) $(OBJ_DIRS) $(SRC_DIR)$(LIB_DIR)	

.PHONY: clean
clean:
	rm -r $(D_DIRS) $(OBJ_DIRS)
	rm -f $(SRC_DIR)$(LIB_DIR)*
	rm -f launcher
