
default: sudoku.cpp preprocess.cpp square.cpp puzzle.cpp population.cpp breed.cpp preprocess.h globals.h square.h puzzle.h population.h breed.h
	g++ -O3 sudoku.cpp preprocess.cpp square.cpp puzzle.cpp population.cpp breed.cpp -std=c++11 -o sudoku -funroll-loops -fopenmp

clean:
	rm -f *.o *~ core
	rm sudoku
