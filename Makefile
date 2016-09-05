all: compile run clean

compile:
	g++ Main.cpp -o SMP -g

run:
	./SMP

clean:
	rm SMP

debug:
	valgrind ./SMP
