EXEC := Reco
SRC  := $(wildcard *.cxx)
OBJ  := $(patsubst %.cxx,%.o,$(SRC))

CXX      := clang++
CPPFLAGS := -g -O3 -std=c++11 $(shell root-config --cflags)
LDFLAGS  := -g $(shell root-config --ldflags)
LDLIBS   := $(shell root-config --libs)

$(EXEC): $(OBJ)
	@echo Linking into $@ ... Done ;$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^
	-@cd ..;rm -rf $(EXEC);ln -s src/$(EXEC) $(EXEC)

%.o: %.cxx
	@echo Compiling source file $< into $@ ;$(CXX) $(CPPFLAGS) -c $<;

%.o: %.h

.PHONY: clean cleanroot

clean:
	@echo Cleaned;rm -rf $(EXEC) $(OBJ)
	-@rm -rf ../$(EXEC)
