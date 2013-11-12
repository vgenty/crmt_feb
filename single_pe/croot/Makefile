all: main.cxx
	-@clang++ -std=c++11 -o fit main.cxx $(shell root-config --cflags --glibs)
clean:
	-@rm -rf fit
