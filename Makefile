
C := $(wildcard *.c)
O := $(C:.c=.o)

all: $(O)
	gcc $^ -o t.out

%.o: %.c
	gcc -c $^ -o $@

clean:
	rm -rf *.o tetris

