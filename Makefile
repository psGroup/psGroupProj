IDIR=include
CC=gcc
CFLAGS=-c -I -Wall
LDFLAGS= -lm -I$(IDIR)

SOURCES=predators_main.c \
		src/dataStructiars.c \
		src/equations.c \
		src/global_values.c \
		src/printFunctions.c 

TESTSOURCES=unit_testing.c \
	src/dataStructiars.c \
	src/equations.c \
	src/global_values.c \
	src/printFunctions.c 

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main.o

TEST_OBJECTS=$(TESTSOURCES:.cpp=.o)
TEST_EXECUTABLE=test.o

all: $(SOURCES) $(EXECUTABLE) $(TESTSOURCES) $(TEST_EXECUTABLE) directories
    
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(TEST_EXECUTABLE): $(TEST_OBJECTS)
	$(CC) $(LDFLAGS) $(TEST_OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

directories:
	mkdir -p outDirs/haresDensity
	mkdir -p outDirs/haresPPM
	mkdir -p outDirs/pumasPPM
	mkdir -p outDirs/pumasDensity
	mkdir -p outDirs/togetherPPM
	mkdir -p test_hares_directory
	mkdir -p test_pumas_directory
	mkdir -p test_together_directory
	echo > "test_file.dat";

clean :
		rm -r outDirs/haresDensity
		rm -r outDirs/haresPPM
		rm -r outDirs/pumasDensity
		rm -r outDirs/pumasPPM
		rm -r outDirs/togetherPPM
		rm *.o
