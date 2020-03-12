#CFLAGS := g++ -Wall -MD -MMD
SRC_DIR := src/
LIB_DIR := a_libr/

SOURCE_DIRS := my_simple_computer my_term UI big_chars
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
compile: my_simple_computer my_term big_chars UI launcher

.PHONY : my_simple_computer
my_simple_computer:
	cd src/my_simple_computer/ && make all
.PHONY : my_term
my_term:
	cd src/my_term/ && make all
.PHONY : UI
UI:	
	cd src/UI/ && make all
.PHONY : big_chars
big_chars:
	cd src/big_chars/ && make all

.PHONY : launcher
launcher:
	cd src/ && make all

#.PHONY : compile
#compile: $(patsubst %.cpp, %.o, $(SEARCH_WILDCARD))
	
#%.o: %.cpp 
#	$(CFLAGS) -c -o $@ $<

.PHONY : makeDirs
makeDirs:
	mkdir -p $(D_DIRS) $(OBJ_DIRS)	

.PHONY: clean
clean:
	rm -r $(D_DIRS) $(OBJ_DIRS)
	rm -f $(SRC_DIR)$(LIB_DIR)*
	rm -f launcher
