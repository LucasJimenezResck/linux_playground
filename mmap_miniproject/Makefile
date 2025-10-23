.PHONY: all clean
EXE=01_allocating_memory 02_reading_files

all: $(EXE)

#@ means executable file and < means source
%: %.c
	gcc -Wall -ggdb -o $@ $<

clean:
	rm -rfv $(EXE)