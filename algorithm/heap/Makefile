
all:test-int test-string

test-int:
	g++ -D__TEST_INT__ -g heap.cpp -o iheap

test-string:
	g++ -D__TEST_STRING__ -g heap.cpp -o sheap

clean:
	rm -rf core iheap sheap
