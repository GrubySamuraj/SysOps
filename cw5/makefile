OUTPUT_DIR = output
CC = gcc
DIR = $(notdir $(CURDIR))
OUTPUT_TAR = cw5.tar.gz

CFLAGS = -Wall -pedantic -O 
LFLAGS = -Wall -std=c99 -pedantic -O 
SOURCES = cw5a.c cw5b.c parent.c child.c
CW5B = cw5b.c
READFILE ?= producent.txt
WRITEFILE ?= consumer.txt
CHILD = output/child
PARENT = output/parent
EXECUTABLES = $(SOURCES:%.c=$(OUTPUT_DIR)/%)

all: $(EXECUTABLES)

clean:
	rm -rf $(OUTPUT_DIR)

$(OUTPUT_DIR)/%: %.c
	mkdir -p $(OUTPUT_DIR)
	$(CC) $(LFLAGS) -o $@  $^
	$(CC) $(CFLAGS) $< -o $@

run_cw5a: $(OUTPUT_DIR)/cw5a
	./$< $(READFILE) $(WRITEFILE) 

run_cw5b: $(OUTPUT_DIR)/cw5b
	$(CC) $(LFLAGS) -o $(OUTPUT_DIR)/cw5b cw5b.c
	$(CC) $(LFLAGS) -o $(OUTPUT_DIR)/child child.c
	$(CC) $(LFLAGS) -o $(OUTPUT_DIR)/parent parent.c
	$(CC) $(CFLAGS) cw5b.c -o $(OUTPUT_DIR)/cw5b
	$(CC) $(CFLAGS) child.c -o $(OUTPUT_DIR)/child
	$(CC) $(CFLAGS) parent.c -o $(OUTPUT_DIR)/parent
#./$(OUTPUT_DIR)/cw5b $(READFILE) $(WRITEFILE) $(CHILD) $(PARENT)
	mkfifo ./fifarafa
	xterm -hold -title PRODUCENT -e ./output/child ./fifarafa  producent.txt &
	xterm -hold -title KONSUMENT -e ./output/parent ./fifarafa  consumer.txt
	diff -s ./producent.txt ./consumer.txt
	rm ./fifarafa

#xterm -hold -title PRODUCENT -e ./producent.x mojfifo dane.txt &
#xterm -hold -title KONSUMENT -e ./konsument.x mojfifo wyniki.txt

tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )