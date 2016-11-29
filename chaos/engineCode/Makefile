CPPFLAGS= -Wall -g -O0 -I /usr/local/include/ -I /usr/include/ -I /usr/local/include -I /usr/local/ssl/include
UNAME = $(shell uname)
ifeq ($(UNAME), Linux)
	GMAKE = make
	LIB = -L/usr/lib -L/usr/local/lib -L/usr/local/ssl/lib/ -L/usr/lib64/ -L/usr/local/mongo/lib/ -levent  -lmsgpack -lmongoclient -lboost_thread -lboost_system -lboost_program_options -lboost_filesystem -lpthread -lreadline -lrt -lz -loauth -lcurl -lssl -lidn -lcrypto -ldl

else
	GMAKE = gmake
	LIB=--static -L/usr/local/lib -lreadline -levent -liconv -lz -lmongoclient -lboost_thread -lboost_system -lboost_filesystem -lboost_program_options -lpthread -lmsgpack -loauth -lcurl -lssl  -lidn -lcrypto -lintl
endif
CC=g++

SRC=callout.cpp hash.cpp lmemory.cpp luacall.cpp main.cpp netlib.cpp api.cpp cjson.cpp lstack.cpp base64.cpp scene.cpp lscene.cpp msgpackLua.cpp

OBJ=${SRC:%.cpp=%.o}

all: depend lua luamongo main

depend:
	@$(CC) -MM $(CPPFLAGS) $(SRC)  > .depend
-include .depend

luamongo:
ifeq ($(UNAME), Linux)
	(cd luamongodb/ && make)
else
	(cd luamongodb/ && gmake)
endif

main: $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJ) -o $@ posix/lposix.a  ./luamongodb/mongo.a $(LIB) 
	cp $@ $@.nostrip
	#strip $@
	cp $@ /home/public

clean:
	-rm -rf *.o main .depend main.nostrip
ifeq ($(UNAME), Linux)
	(cd luamongodb/ && make clean)
else
	(cd luamongodb/ && gmake clean)
endif
