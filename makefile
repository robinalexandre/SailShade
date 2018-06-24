CXX=g++-7
CXXFLAGS=-Wall -Wextra -pedantic -O3 -std=c++11 -fopenmp 
LDFLAGS=-O3 -lSolTrack -lm -fopenmp
EXEC=sailshadow
SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)

all: $(EXEC)

sailshadow: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CXXFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
			
