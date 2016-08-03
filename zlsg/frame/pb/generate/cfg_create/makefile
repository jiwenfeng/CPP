include ../../../../xxsq.mk

INC = $(CMQ_INC) $(BASE_INC) $(PB_INC) $(PROTOC_INC) $(AMTSVR_INC) $(MYSQL_INC) 
LIB = $(CMQ_LIB) $(BASE_LIB) $(PB_LIB) $(PROTOC_LIB) $(MYSQL_LIB) -lpthread -lz -ldl

#-----------------------------------------------------------
BINARY = cfgcreate
BASEOBJS = $(patsubst ./%.cc, ./%.o, $(wildcard ./*.cc))

#-----------------------------------------------------------

.PHONY: all clean

all: $(BINARY)

$(BINARY): $(BASEOBJS)
	g++ $(C_ARGS) -o $@ $(BASEOBJS) $(LIB)
	mv $(BINARY) ../ -f

clean:
	rm -f *.o $(BINARY)

.cc.o:
	g++ $(C_ARGS) $(INC) -c $<
