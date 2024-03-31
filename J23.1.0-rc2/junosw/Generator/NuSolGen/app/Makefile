CXX := g++
CPPFLAG := $(shell root-config --cflags) $(shell clhep-config --include) -I../src
LDFLAG := $(shell root-config --libs)  $(shell clhep-config --libs)
NuSolGen.exe: main.o SolarNeutrinoSpectrum.o
	g++ -o $@ $(LDFLAG) $^ 

main.o: main.cc ../src/SolarNeutrinoSpectrum.hh
	g++ -o $@ -c $(CPPFLAG) $<

SolarNeutrinoSpectrum.o: ../src/SolarNeutrinoSpectrum.cc ../src/SolarNeutrinoSpectrum.hh
	g++ -o $@ -c $(CPPFLAG) $<
