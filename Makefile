CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -O2

SRCS = src/Polynomial.cpp
OBJS = src/Polynomial.o

TARGET = PolynomialToolkit.exe
TEST_TARGET = runTests.exe

all: $(TARGET) $(TEST_TARGET)

$(TARGET): main.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_TARGET): tests/test_polynomial.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f main.o src/Polynomial.o tests/test_polynomial.o $(TARGET) $(TEST_TARGET)
