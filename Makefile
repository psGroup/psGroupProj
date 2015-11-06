CC = gcc 
all: directories target

directories:
	mkdir -p haresDensity
	mkdir -p haresPPM
	mkdir -p pumasPPM
	mkdir -p pumasDensity
	mkdir -p togetherPPM

LIBS = -lm
OBJH= printFunctions.h dataStructiars.h equations.h global_values.h
OBJC= printFunctions.c dataStructiars.c equations.c predators_main.c
OBJO= printFunctions.o dataStructiars.o equations.o predators_main.o

$(OBJO): $(OBJC) $(OBJH)  
	$(CC) $(LIBS) $(OBJC)

target : $(OBJO)
	./a.out test.dat


.PHONY : clean
clean:
	rm main.o dataStructiars.o equations.o printFunctions.o
	rm -f
	rm -r haresDensity
	rm -r haresPPM
	rm -r pumasDensity
	rm -r pumasPPM
	rm -r togetherPPM
