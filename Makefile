CPP = clang++
CPPFLAGS = -g -std=c++11 -Wall

all: principal

principal.o: principal.cpp extSort.hpp
extSort.o: extSort.cpp extSort.hpp

principal: principal.o extSort.o
	$(CPP) $(CPPFLAGS) -o $@  $^

clean:
	rm -f principal *.o
