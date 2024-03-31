all: PixelConversion
PixelConversion: PixelConversion.cc
	g++ -o PixelConversion PixelConversion.cc PMTinfo.h -O0 `root-config --cflags` `root-config --libs`

clean:
	rm ./PixelConversion

