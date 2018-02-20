# Podéis usar este Makefile como referencia. Vuestro codigo debe poderse 
# compilar con GCC usando los flags que ya estan especificados en CFLAGS.
# Podéis experimentar con la variable OPT para añadir opciones adicionales.

CC = gcc 
OPT = -O0
CFLAGS = -Wall -std=gnu99 $(OPT)
LDFLAGS = -Wall
LDLIBS = 

targets = benchmark
objects = benchmark.o dgemm.o

.PHONY : default
default : all

.PHONY : all
all : clean $(targets)

benchmark : benchmark.o dgemm.o
	$(CC) -o $@ $^ $(LDLIBS)

%.o : %.c
	$(CC) -c $(CFLAGS) $<

.PHONY : clean
clean:
	rm -f $(targets) $(objects)

