CXX = g++
CXXFLAGS = -Wall -std=c++17 -O3

TARGET = matrix_multiplication

SRCS = matrix_multiplication.cpp \
	algorithms/naive.cpp \
	algorithms/strassen.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)