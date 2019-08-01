# Personal Makefile Template.
CXX = g++
CXX_FLAGS = -std=c++17 -Wconversion -Wall -Werror -Wextra -pedantic
OPT = -O3 -DNDEBUG
DEBUG = -g3 -DDEBUG

# Put executable and linked file names. Do NOT include extensions! 
EXECUTABLE = sudoku
LINKED_FILES = square grid

# File list expansions.
LINKED_CPP = $(foreach file, $(LINKED_FILES), $(file).cpp;)
LINKED_O = $(foreach file, $(LINKED_FILES), $(file).o;)

# Build optimized executable.
release : $(EXECUTABLE).cpp $(LINKED_CPP)
	$(CXX) $(CXX_FLAGS) $(OPT) -c $(EXECUTABLE).cpp $(LINKED_CPP);
	$(CXX) $(CXX_FLAGS) $(OPT) $(EXECUTABLE).o $(LINKED_O) -o $(EXECUTABLE);

# Build with debug features.
debug : $(EXECUTABLE).cpp $(LINKED_CPP)
	$(CXX) $(CXX_FLAGS) $(DEBUG) -c $(EXECUTABLE).cpp $(LINKED_CPP)
	$(CXX) $(CXX_FLAGS) $(DEBUG) $(EXECUTABLE).cpp $(LINKED_CPP) -o $(EXECUTABLE)_debug;

# Remove executable and all runtime outputs.
.PHONY : clean
clean : 
	rm -f $(EXECUTABLE) $(EXECUTABLE)_debug *.o *.txt
