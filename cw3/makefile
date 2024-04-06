OUTPUT_DIR = output
CC = gcc
DIR = $(notdir $(CURDIR))
OUTPUT_TAR = cw3.tar.gz

CFLAGS = -Wall -pedantic -O 
LFLAGS = -Wall -std=c99 -pedantic -O 
SOURCES = cw3a.c cw3b.c cw3c.c
CW3C2 = forkleader
EXECUTABLES = $(SOURCES:%.c=$(OUTPUT_DIR)/%)

all: $(EXECUTABLES)

clean:
	rm -rf $(OUTPUT_DIR)

$(OUTPUT_DIR)/%: %.c
	mkdir -p $(OUTPUT_DIR)
	$(CC) $(LFLAGS) -o $@  $^
	$(CC) $(CFLAGS) $< -o $@

run_cw3a: $(OUTPUT_DIR)/cw3a
	./$(OUTPUT_DIR)/cw3a 0 20

run_cw3c: $(OUTPUT_DIR)/cw3a
	$(CC) $(LFLAGS) -o $(OUTPUT_DIR)/cw3c cw3c.c
	$(CC) $(CFLAGS) cw3c.c -o $(OUTPUT_DIR)/cw3c
	$(CC) $(LFLAGS) -o $(OUTPUT_DIR)/$(CW3C2) $(CW3C2).c
	$(CC) $(CFLAGS) $(CW3C2).c -o $(OUTPUT_DIR)/$(CW3C2)
	./$(OUTPUT_DIR)/cw3c

run_%: $(OUTPUT_DIR)/%
	./$<
	
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
