CC=gcc
CFLAGS=-c -Wall -Wextra -O2
LDFLAGS=
SOURCES=aloha.c file_output.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=aloha

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

file_output.o : file_output.h

.c.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY:
clean:
	rm -rf $(OBJECTS)

experiment: $(EXECUTABLE)
	bash experiments/p_vs_delays.sh
	gnuplot experiments/plot_p_vs_delays.gnuplot -p
