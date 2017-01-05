CC=g++
CPPFLAGS=-g -Wall
SRC=$(wildcard *.cpp)
OBJ=$(patsubst %.cpp, %.o, $(SRC))
TARGET=$(patsubst %.o, %.out, $(OBJ))

all:$(TARGET)
	@echo "done"

%.out:%.o
	$(CC) $(CFLAGS) $^ -o $@ -std=c++11

%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -std=c++11

clean:
	rm -rf $(OBJ) $(TARGET)

