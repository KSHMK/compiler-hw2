CC = gcc
CFLAGS = -g -Wall
TARGET = parser_201820682.out
OBJS = main.o lex.yy.o parser.o process.o data_set.o 

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lfl

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

lex.yy.c:
	flex lex.l

clean:
	rm -f *.o
	rm -f lex.yy.c
	rm -f $(TARGET)