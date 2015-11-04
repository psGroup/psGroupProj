# Compiles the main file, displays ppms
all : target run

#doxygenT: doxygenfinaltemplate.c
	#doxygen doxygenfinaltemplate.c doxygenT 

compiler: *.c
	gcc -o target *.c -I.

run: target
	./target Test.dat

# type "make clean" into the command line to clear the contents of the data directories
.PHONY : clean
clean :
		rm -f haresOut/*
		rm -f haresValues/*
		rm -f pumasOut/*
		rm -f pumasValues/*
		rm -f togetherOut/*
