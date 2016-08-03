include ../../xxsq.mk

C_FILE = $(wildcard *.cc )
OBJ_FILE = $(patsubst %.cc, %.o, $(C_FILE))
OUT_FILE = ./game.a

COMP_INC = $(SCENE_INC) $(ROLE_INC) $(CONFIG_INC) $(NAVMESH_INC) $(BASE_INC) $(PB_INC) $(PROTOC_INC) $(AMTSVR_INC)
COMP_LIB = $(SCENE_LIB) $(ROLE_LIB) $(CONFIG_LIB) $(NAVMESH_LIB) $(BASE_LIB) $(PB_LIB) $(PROTOC_LIB) -lpthread -lz

all:$(OUT_FILE)
$(OUT_FILE): $(OBJ_FILE)
	$(AR) rc $(OUT_FILE) $(OBJ_FILE)
	
%.o:%.cc
	$(CXX) $(C_ARGS) -o $@ $(COMP_INC) -c $<
	
clean:
	rm -f $(OUT_FILE) $(OBJ_FILE)
