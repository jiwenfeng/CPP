CC=g++
CPPFLAGS=-g -Wall
CPP_FILES=$(wildcard *.cpp)
OBJ_FILES=$(patsubst %.cpp, %.o, $(CPP_FILES))
TARGET=$(subst .o, , $(OBJ_FILES))

all:$(TARGET)
	@echo "done"

$(TARGET):$(OBJ_FILES)
	$(CC) $(CPPFLAGS) $< -o $@

%.o:%.cpp
	$(CC) -c $< -o $@ -std=c++11

clean:
	rm -rf $(OBJ_FILES) $(TARGET)
