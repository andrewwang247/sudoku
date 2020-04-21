# Compiler and flags.
CXX := g++ -std=c++17
FLAGS := -Wall -Werror -Wextra -Wconversion -pedantic -Wfloat-equal -Wduplicated-branches -Wduplicated-cond -Wshadow -Wdouble-promotion -Wundef
OPT := -O3 -DNDEBUG
DEBUG := -ggdb3 -DDEBUG

# Put executable and linked file names. Do NOT include extensions! 
EXE = sudoku
LINKED_FILES = square grid

# File list expansions.
LINKED_CPP = $(foreach file, $(LINKED_FILES), $(file).cpp)
LINKED_O = $(foreach file, $(LINKED_FILES), $(file).o)

# Build optimized executable.
release : $(EXE).cpp $(LINKED_CPP)
	$(CXX) $(FLAGS) $(OPT) -c $(EXE).cpp $(LINKED_CPP)
	$(CXX) $(FLAGS) $(OPT) $(EXE).o $(LINKED_O) -o $(EXE)

# Build with debug features.
debug : $(EXE).cpp $(LINKED_CPP)
	$(CXX) $(FLAGS) $(DEBUG) -c $(EXE).cpp $(LINKED_CPP)
	$(CXX) $(FLAGS) $(DEBUG) $(EXE).cpp $(LINKED_CPP) -o $(EXE)

# Remove executable and all runtime outputs.
.PHONY : clean
clean : 
	rm -r $(EXE) $(LINKED_O) $(EXE).o
