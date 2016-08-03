include ../../../xxsq.mk

CPP_FILE = $(wildcard *.cc )
OBJ_FILE = $(patsubst %.cc, %.o, $(CPP_FILE))
OUT_FILE = ./pb_server.a

COMP_INC = $(PROTOC_INC) 
COMP_LIB = $(PROTOC_LIB) -lpthread -lz

all:$(OUT_FILE)
$(OUT_FILE): $(OBJ_FILE)
	$(AR) rc $(OUT_FILE) $(OBJ_FILE)

%.o:%.cc
	$(CXX) $(C_ARGS) -o $@ $(COMP_INC) -c $<

clean:
	rm -f $(OUT_FILE) $(OBJ_FILE) 
