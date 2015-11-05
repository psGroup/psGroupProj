# Compiles the main file, displays ppms
all : directories target run

directories:
	mkdir -p haresDensity
	mkdir -p haresPPM
	mkdir -p pumasPPM
	mkdir -p pumasDensity
	mkdir -p togetherPPM

target: *.c
	gcc -o target *.c -I.

#.PHONY : run
#run: target
#	./target Test.dat

# type "make clean" into the command line to clear the directories
.PHONY : clean
clean :
		rm -r haresDensity
		rm -r haresPPM
		rm -r pumasDensity
		rm -r pumasPPM
		rm -r togetherPPM
