all: compile run clean

compile:
	g++ Main.cpp -o SMP -g

run:
	./SMP

clean:
	rm SMP

debug:
	g++ Main.cpp -o SMP -g
	valgrind ./SMP
	rm SMP