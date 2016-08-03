include ../../xxsq.mk

C_FILE = $(wildcard *.cc )
OBJ_FILE = $(patsubst %.cc, %.o, $(C_FILE))
OUT_FILE = ./scene.a

COMP_INC = $(ROLE_INC) $(CONFIG_INC) $(NAVMESH_INC) $(BASE_INC) $(PB_INC) $(PROTOC_INC) $(AMTSVR_INC)  $(CENTER_FAMILY_INC) $(CENTER_ARENA_INC)
COMP_LIB = $(ROLE_LIB) $(CONFIG_LIB) $(NAVMESH_LIB) $(BASE_LIB) $(PB_LIB) $(PROTOC_LIB) $(CENTER_FAMILY_LIB) -lpthread $(CENTER_ARENA_INC) -lz

all:$(OUT_FILE)
$(OUT_FILE): $(OBJ_FILE)
	$(AR) rc $(OUT_FILE) $(OBJ_FILE)
	
%.o:%.cc
	$(CXX) $(C_ARGS) -o $@ $(COMP_INC) -c $<
	
clean:
	rm -f $(OUT_FILE) $(OBJ_FILE)
