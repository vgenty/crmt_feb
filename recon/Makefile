EXEC := RecoCRMT
SRC  := $(wildcard *.cxx)
OBJ  := $(patsubst %.cxx,%.o,$(SRC))

CXX      := clang++
CPPFLAGS := -g -O3 -Wall -std=c++11 $(shell root-config --cflags)
LDFLAGS  := -g $(shell root-config --ldflags)
LDLIBS   := $(shell root-config --libs)

$(EXEC): $(OBJ)
	@echo Linking into $(EXEC)... Done;$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

%.o: %.cxx
	@echo Compiling $<...;$(CXX) $(CPPFLAGS) -c $<;

%.o: %.h

.PHONY: clean cleanroot

clean:
	@echo Cleaned;rm -rf $(EXEC) $(OBJ)

cleanroot:
	@echo Cleaned;rm -rf $(EXEC) $(OBJ) *.root
