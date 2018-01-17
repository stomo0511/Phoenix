CXX =	/usr/local/bin/g++ -fopenmp
# for DEBUG
#CXXFLAGS =	-DDEBUG -g
# for Performance evaluation
CXXFLAGS =	-O3

OBJS =		Phoenix.o

all:phoe

phoe: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

clean:
	rm -f VC $(OBJS)
