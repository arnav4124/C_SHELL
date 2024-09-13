


CC = gcc


CFLAGS = -w -g


LIBS = -lm


OUTPUT = a.out


SRCS = $(wildcard *.c)

OBJS = $(SRCS:.c=.o)


all: $(OUTPUT)


$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJS) $(OUTPUT)
