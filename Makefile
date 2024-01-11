CXX = gcc
CXXFLAGS = -std=c11 -lm

SRCS = main.c

EXEC = digitalForensics


all:
	$(CXX) -o $(EXEC) $(CXXFLAGS) $(SRCS)

clean:
	rm $(EXEC)
