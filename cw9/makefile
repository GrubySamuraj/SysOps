OUTPUT_DIR = output
CC = gcc
DIR = $(notdir $(CURDIR))
OUTPUT_TAR = cw9.tar.gz
CFLAGS = -Wall -pedantic -O -pthread
LFLAGS = -Wall -std=c99 -pedantic -O -pthread
SOURCES = main.c
NUMOFTHREADS = 10
NUMOFITERATIONS = 2
EXECUTABLES = $(SOURCES:%.c=$(OUTPUT_DIR)/%)

all: $(EXECUTABLES)

clean:
	rm -rf $(OUTPUT_DIR)

$(OUTPUT_DIR)/%: %.c
	mkdir -p $(OUTPUT_DIR)
	$(CC) $(LFLAGS) -o $@  $^ 
	$(CC) $(CFLAGS) $< -o $@

run_%: $(OUTPUT_DIR)/%
	./$< $(NUMOFTHREADS) $(NUMOFITERATIONS)

run: $(OUTPUT_DIR)/main
	./$(OUTPUT_DIR)/main $(NUMOFTHREADS) $(NUMOFITERATIONS)
	
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
