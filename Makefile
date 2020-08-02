CXX=g++
CXXFLAGS=-std=c++14 -Wall -Werror -g -O2 \
	$(shell pkg-config --cflags capnp)
LDFLAGS=\
	$(shell pkg-config --libs capnp)

SRC=$(wildcard *.cc) msg.capnp.c++
OBJ=$(SRC:.cc=.o)

cppexample: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# theoretically you could work capnp into the makefile so it
# rebuilds automatically, but transitive dependencies are a pita
# just run `make capnp` before anything else.
.PHONY: capnp
capnp: msg.capnp
	capnp compile -oc++ msg.capnp

.PHONY: run
run: cppexample
	./cppexample

.PHONY: clean
clean:
	rm -f cppexample $(OBJ) msg.capnp.c++ msg.capnp.h
