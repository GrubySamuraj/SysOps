OUTPUT_DIR = output
CC = gcc
DIR = $(notdir $(CURDIR))
OUTPUT_TAR = cw6.tar.gz

CFLAGS = -Wall -pedantic -O -pthread
LFLAGS = -Wall -std=c99 -pedantic -O -pthread
SOURCES = main.c potomny.c
POTOMNY = potomny
EXECUTABLES = $(SOURCES:%.c=$(OUTPUT_DIR)/%)

all: $(EXECUTABLES)

clean:
	rm -rf $(OUTPUT_DIR)

$(OUTPUT_DIR)/%: %.c
	mkdir -p $(OUTPUT_DIR)
	$(CC) $(LFLAGS) -o $@  $^
	$(CC) $(CFLAGS) $< -o $@

run_main: $(OUTPUT_DIR)/main
	$(CC) $(LFLAGS) -o $(OUTPUT_DIR)/main main.c
	$(CC) $(CFLAGS) -o $(OUTPUT_DIR)/main main.c
	$(CC) $(LFLAGS) -o $(OUTPUT_DIR)/$(POTOMNY) $(POTOMNY).c
	$(CC) $(CFLAGS) -o $(OUTPUT_DIR)/$(POTOMNY) $(POTOMNY).c
	./$(OUTPUT_DIR)/main $(OUTPUT_DIR)/$(POTOMNY)

run_%: $(OUTPUT_DIR)/%
	./$<

tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
