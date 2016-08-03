include ../../xxsq.mk

CPP_FILE = $(wildcard *.cc )
OBJ_FILE = $(patsubst %.cc, %.o, $(CPP_FILE))
OUT_FILE = ./navmesh.a

COMP_INC = 
COMP_LIB = 

all:$(OUT_FILE)
$(OUT_FILE): $(OBJ_FILE)
	$(AR) rc $(OUT_FILE) $(OBJ_FILE)

%.o:%.cc
	$(CXX) $(C_ARGS) -fPIC -o $@ $(COMP_INC) -c $<

clean:
	rm -f $(OUT_FILE) $(OBJ_FILE) 
