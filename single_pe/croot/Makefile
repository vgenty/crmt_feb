all: main.cxx
	-@clang++ -std=c++11 -o fit main.cxx $(shell root-config --cflags --glibs) -lSpectrum
clean:
	-@rm -rf fit
