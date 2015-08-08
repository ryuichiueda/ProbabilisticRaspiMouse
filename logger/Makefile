CXX = g++-4.7
TARGET = main
CXXFLAGS = -Wall -O3 --std=c++11 -pthread
LDFLAGS = -lm -pthread
SRCS := $(wildcard *.cc)
OBJS := $(SRCS:.cc=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -f $(TARGET) $(OBJS)
