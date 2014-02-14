SOURCES = $(wildcard *.cpp)

OCTS = $(SOURCES:.cpp=.oct)

all: $(OCTS)

%.oct: %.cpp 
	mkoctfile -lxtract $<
