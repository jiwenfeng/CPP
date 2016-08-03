C_FILE = $(wildcard *.cpp)
OBJ_FILE = $(patsubst %.cpp, %.o, $(C_FILE))
OUT_FILE = ./libluatinker.a

COMP_INC = 
COMP_LIB = 

all:$(OUT_FILE)
$(OUT_FILE): $(OBJ_FILE)
	$(AR) rc $(OUT_FILE) $(OBJ_FILE)
	
%.o:%.cpp
	$(CXX) $(C_ARGS) -o $@ $(COMP_INC) -c $<
	
clean:
	rm -f $(OUT_FILE) $(OBJ_FILE)
