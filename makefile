CC = gcc
CFLAGS = -std=c99 -Wall -pedantic-errors -I.

SRCS = main.c matfile.c 
HDRS = matfile.h
OBJS = $(SRCS:.c = .o)

EXE = matfile

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXE)

%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(EXE)
