CXX = g++
CXXFLAGS = -Wall -std=c++17 -O3

TARGET = sorting

SRCS = sorting.cpp \
		algorithms/sort.cpp \
		algorithms/mergesort.cpp \
		algorithms/quicksort.cpp \
		algorithms/patiencesort.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
		$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
		$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
		rm -f $(OBJS) $(TARGET)