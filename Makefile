all: main.cpp Formula.o
	g++ -Wall -g Formula.o main.cpp -o test
Formula.o: Formula.cpp Formula.h
	g++ -Wall -g -c Formula.cpp
Sfe.o: Sfe.cpp  
	g++ -Wall -g -c Sfe.cpp 
element.o:element.cpp
	g++ -Wall -g -c element.cpp
Cb.o: Cb.cpp
	g++ -Wall -g -c Cb.cpp
test: main_sfe.cpp Sfe.o element.o Cb.o
	g++ -Wall -g Sfe.o element.o Cb.o main_sfe.cpp -o a
clean:
	rm -f all test
	rm -f Formula.o
	rm -f Sfe.o
	rm -f element.o
	rm -f Cb.o

