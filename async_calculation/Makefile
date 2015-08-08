CXX = g++-4.7
TARGET = main
CXXFLAGS = -Wall -O3 --std=c++11
LDFLAGS = -lm
SRCS := $(wildcard *.cc)
OBJS := $(SRCS:.cc=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) 

clean:
	rm -f $(TARGET) $(OBJS)
