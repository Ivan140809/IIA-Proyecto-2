CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

all:
	mkdir -p build
	$(CXX) $(CXXFLAGS) src/rules/*.cpp src/variables/*.cpp src/sets/*.cpp src/engine/*.cpp src/main.cpp -o build/fuzzy
clean: 
	rm -rf build


