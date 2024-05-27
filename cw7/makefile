OUTPUT_DIR = output
CC = gcc
DIR = $(notdir $(CURDIR))
OUTPUT_TAR = cw7.tar.gz

CFLAGS = -Wall -pedantic -O -pthread
LFLAGS = -Wall -std=c99 -pedantic -O -pthread
SOURCES = main.c potomny.c
KONSUMENT = konsument
PRODUCENT = producent
READFILE = konsument.txt
WRITEFILE = producent.txt
EXECUTABLES = $(SOURCES:%.c=$(OUTPUT_DIR)/%)

all: $(EXECUTABLES)

clean:
	rm -rf $(OUTPUT_DIR)

$(OUTPUT_DIR)/%: %.c
	mkdir -p $(OUTPUT_DIR)
	$(CC) $(LFLAGS) -o $@  $^
	$(CC) $(CFLAGS) $< -o $@

run_cw7: $(OUTPUT_DIR)/main
	$(CC) $(LFLAGS) -o $(OUTPUT_DIR)/cw7 cw7.c
	$(CC) $(CFLAGS) -o $(OUTPUT_DIR)/cw7 cw7.c
	$(CC) $(LFLAGS) -o $(OUTPUT_DIR)/$(PRODUCENT) $(PRODUCENT).c
	$(CC) $(CFLAGS) -o $(OUTPUT_DIR)/$(PRODUCENT) $(PRODUCENT).c
	$(CC) $(LFLAGS) -o $(OUTPUT_DIR)/$(KONSUMENT) $(KONSUMENT).c
	$(CC) $(CFLAGS) -o $(OUTPUT_DIR)/$(KONSUMENT) $(KONSUMENT).c
	./$(OUTPUT_DIR)/cw7 $(OUTPUT_DIR)/$(PRODUCENT) $(OUTPUT_DIR)/$(KONSUMENT) $(READFILE) $(WRITEFILE)

run_%: $(OUTPUT_DIR)/%
	./$<

tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
