include ../../../xxsq.mk

CPP_FILE = $(wildcard *.cc )
OBJ_FILE = $(patsubst %.cc, %.o, $(CPP_FILE))
OUT_FILE = ./center_randnpc.a

COMP_INC = $(ROLE_INC) $(CONFIG_INC) $(BASE_INC) $(PB_INC) $(PROTOC_INC) $(AMTSVR_INC)
COMP_LIB = $(ROLE_LIB) $(CONFIG_LIB) $(BASE_LIB) $(PB_LIB) $(PROTOC_LIB) -lpthread -lz

all:$(OUT_FILE)
$(OUT_FILE): $(OBJ_FILE)
	$(AR) rc $(OUT_FILE) $(OBJ_FILE)

%.o:%.cc
	$(CXX) $(C_ARGS) -fPIC -o $@ $(COMP_INC) -c $<

clean:
	rm -f $(OUT_FILE) $(OBJ_FILE) 
	
