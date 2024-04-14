OUTPUT_DIR = output
CC = gcc
DIR = $(notdir $(CURDIR))
OUTPUT_TAR = cw3.tar.gz

CFLAGS = -Wall -pedantic -O 
LFLAGS = -Wall -std=c99 -pedantic -O 
SOURCES = cw4.c
READFILE = producent.txt
WRITEFILE = consumer.txt
EXECUTABLES = $(SOURCES:%.c=$(OUTPUT_DIR)/%)

all: $(EXECUTABLES)

clean:
	rm -rf $(OUTPUT_DIR)

$(OUTPUT_DIR)/%: %.c
	mkdir -p $(OUTPUT_DIR)
	$(CC) $(LFLAGS) -o $@  $^
	$(CC) $(CFLAGS) $< -o $@

run_cw4: $(OUTPUT_DIR)/cw4
	./$< $(READFILE) $(WRITEFILE)
	diff -s ./$(READFILE) ./$(WRITEFILE)
	
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
