CXX=g++
CXXFLAGS=-Wall -std=c++11 -O2 -DNDEBUG -fpermissive
LDFLAGS=-lcapstone -lbfd-multiarch
INCLUDE_PATH = -I../capstone/include
LIBRARY_PATH = -L../capstone

SRC=$(wildcard *.cc)
OBJ=$(patsubst %.cc, obj/%.o, $(SRC))
BIN=nucleus

A_SRC = $(filter-out loader.cc,$(SRC))
A_OBJ=$(patsubst %.cc, obj/%.o, $(A_SRC))

.PHONY: all clean setup

all: $(BIN) $(BIN).a

$(OBJ): | obj

obj:
	@mkdir -p $@

obj/%.o: %.cc %.h
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATH) -c -o $@ $<

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)

lib$(BIN).a: $(A_OBJ)
	ar rcs $@ $(A_OBJ)

setup:
	sudo apt install binutils-multiarch-dev libcapstone-dev

clean:
	rm -f $(OBJ)
	rm -Rf obj
	rm -f $(BIN)
	rm -f libnucleus.a

